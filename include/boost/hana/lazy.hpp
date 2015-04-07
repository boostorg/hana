/*!
@file
Defines `boost::hana::Lazy`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LAZY_HPP
#define BOOST_HANA_LAZY_HPP

#include <boost/hana/fwd/lazy.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/comonad.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/wrong.hpp>
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // eval
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct eval_impl : eval_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct eval_impl<T, when<condition>> : default_ {
        template <typename Expr>
        static constexpr auto eval_helper(Expr&& expr, int)
            -> decltype(static_cast<Expr&&>(expr)())
        { return static_cast<Expr&&>(expr)(); }

        template <typename Expr>
        static constexpr auto eval_helper(Expr&& expr, long)
            -> decltype(static_cast<Expr&&>(expr)(hana::id))
        { return static_cast<Expr&&>(expr)(hana::id); }

        template <typename Expr>
        static constexpr auto eval_helper(Expr&&, ...) {
            static_assert(detail::wrong<Expr>{},
            "hana::eval(expr) requires the expression to be Lazy, "
            "a nullary Callable or a unary Callable that may be "
            "called with hana::id");
        }

        template <typename Expr>
        static constexpr decltype(auto) apply(Expr&& expr)
        { return eval_helper(static_cast<Expr&&>(expr), int{}); }
    };

    //////////////////////////////////////////////////////////////////////////
    // lazy
    //////////////////////////////////////////////////////////////////////////
    template <typename F, typename Args>
    struct _lazy_apply;

    template <typename F, typename ...Args>
    struct _lazy_apply<F, detail::closure_impl<Args...>> : operators::adl {
        F function;
        detail::closure_impl<Args...> args;

        _lazy_apply(_lazy_apply const&) = default;
        _lazy_apply(_lazy_apply&&) = default;
        _lazy_apply(_lazy_apply&) = default;

        template <typename F_, typename ...Args_,
            typename = decltype(F(detail::std::declval<F_>())),
            typename = decltype(detail::closure_impl<Args...>(
                                    detail::std::declval<Args_>()...))
        >
        explicit constexpr _lazy_apply(F_&& f, Args_&& ...x)
            : function(static_cast<F_&&>(f))
            , args(static_cast<Args_&&>(x)...)
        { }

        using hana = _lazy_apply;
        using datatype = Lazy;
    };

    template <typename X>
    struct _lazy_value : operators::adl {
        X value;

        _lazy_value(_lazy_value const&) = default;
        _lazy_value(_lazy_value&&) = default;
        _lazy_value(_lazy_value&) = default;

        template <typename X_, typename = decltype(X(detail::std::declval<X_>()))>
        explicit constexpr _lazy_value(X_&& x)
            : value(static_cast<X_&&>(x))
        { }

        // If this is called, we assume that `value` is in fact a function.
        template <typename ...Args>
        constexpr auto operator()(Args&& ...args) const& {
            return _lazy_apply<
                X, detail::closure<typename detail::std::decay<Args>::type...>
            >{value, static_cast<Args&&>(args)...};
        }

        template <typename ...Args>
        constexpr auto operator()(Args&& ...args) && {
            return _lazy_apply<
                X, detail::closure<typename detail::std::decay<Args>::type...>
            >{detail::std::move(value), static_cast<Args&&>(args)...};
        }

        using hana = _lazy_value;
        using datatype = Lazy;
    };

    //! @cond
    template <typename X>
    constexpr auto _lazy::operator()(X&& x) const {
        return _lazy_value<typename detail::std::decay<X>::type>{
            static_cast<X&&>(x)
        };
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct operators::of<Lazy>
        : operators::of<Monad>
    { };

    //////////////////////////////////////////////////////////////////////////
    // eval for Lazy
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct eval_impl<Lazy> {
        // _lazy_apply
        template <typename F, typename ...Args>
        static constexpr decltype(auto)
        apply(_lazy_apply<F, detail::closure_impl<Args...>> const& expr)
        { return expr.function(static_cast<Args const&>(expr.args).get...); }

        template <typename F, typename ...Args>
        static constexpr decltype(auto)
        apply(_lazy_apply<F, detail::closure_impl<Args...>>& expr)
        { return expr.function(static_cast<Args&>(expr.args).get...); }

        template <typename F, typename ...Args>
        static constexpr decltype(auto)
        apply(_lazy_apply<F, detail::closure_impl<Args...>>&& expr) {
            return detail::std::move(expr.function)(
                                    static_cast<Args&&>(expr.args).get...);
        }

        // _lazy_value
        template <typename X>
        static constexpr X const& apply(_lazy_value<X> const& expr)
        { return expr.value; }

        template <typename X>
        static constexpr X& apply(_lazy_value<X>& expr)
        { return expr.value; }

        template <typename X>
        static constexpr X apply(_lazy_value<X>&& expr)
        { return detail::std::move(expr.value); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<Lazy> {
        template <typename Expr, typename F>
        static constexpr auto apply(Expr&& expr, F&& f) {
            return hana::lazy(hana::compose(static_cast<F&&>(f),
                                            hana::eval))
                                        (static_cast<Expr&&>(expr));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<Lazy> {
        template <typename X>
        static constexpr auto apply(X&& x) {
            return _lazy_value<typename detail::std::decay<X>::type>{
                static_cast<X&&>(x)
            };
        }
    };

    template <>
    struct ap_impl<Lazy> {
        template <typename F, typename X>
        static constexpr decltype(auto) apply(F&& f, X&& x) {
            return hana::lazy(hana::on(hana::apply, hana::eval))(
                static_cast<F&&>(f), static_cast<X&&>(x)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<Lazy> {
        template <typename Expr>
        static constexpr decltype(auto) apply(Expr&& expr) {
            return hana::lazy(hana::compose(eval, eval))(
                static_cast<Expr&&>(expr)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comonad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct extract_impl<Lazy> {
        template <typename Expr>
        static constexpr decltype(auto) apply(Expr&& expr)
        { return hana::eval(static_cast<Expr&&>(expr)); }
    };

    template <>
    struct duplicate_impl<Lazy> {
        template <typename Expr>
        static constexpr decltype(auto) apply(Expr&& expr)
        { return hana::lazy(static_cast<Expr&&>(expr)); }
    };

    template <>
    struct extend_impl<Lazy> {
        template <typename Expr, typename F>
        static constexpr decltype(auto) apply(Expr&& expr, F&& f) {
            return hana::lazy(static_cast<F&&>(f))(
                                        static_cast<Expr&&>(expr));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LAZY_HPP
