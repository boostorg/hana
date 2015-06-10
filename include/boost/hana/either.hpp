/*!
@file
Defines `boost::hana::Either`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_HPP
#define BOOST_HANA_EITHER_HPP

#include <boost/hana/fwd/either.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // left
    //////////////////////////////////////////////////////////////////////////
    template <typename X>
    struct _left : operators::adl {
        X value;
        using hana = _left;
        using datatype = Either;

        _left() = default;
        _left(_left const&) = default;
        _left(_left&&) = default;
        _left(_left&) = default;
        template <typename Y, typename = decltype(X(detail::std::declval<Y>()))>
        constexpr _left(Y&& y)
            : value(static_cast<Y&&>(y))
        { }

        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) const&
        { return static_cast<F&&>(f)(value); }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) &
        { return static_cast<F&&>(f)(value); }
#endif

        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) &&
        { return static_cast<F&&>(f)(detail::std::move(value)); }
    };

    //! @cond
    template <typename T>
    constexpr auto _make_left::operator()(T&& t) const {
        return _left<typename detail::std::decay<T>::type>{
            static_cast<T&&>(t)
        };
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // right
    //////////////////////////////////////////////////////////////////////////
    template <typename X>
    struct _right : operators::adl {
        X value;
        using hana = _right;
        using datatype = Either;

        _right() = default;
        _right(_right const&) = default;
        _right(_right&&) = default;
        _right(_right&) = default;
        template <typename Y, typename = decltype(X(detail::std::declval<Y>()))>
        constexpr _right(Y&& y)
            : value(static_cast<Y&&>(y))
        { }

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) const&
        { return static_cast<G&&>(g)(value); }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) &
        { return static_cast<G&&>(g)(value); }
#endif

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) &&
        { return static_cast<G&&>(g)(detail::std::move(value)); }
    };

    //! @cond
    template <typename T>
    constexpr auto _make_right::operator()(T&& t) const {
        return _right<typename detail::std::decay<T>::type>{
            static_cast<T&&>(t)
        };
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <>
        struct of<Either>
            : operators::of<Comparable, Orderable, Monad>
        { };
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Either, Either> {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) apply(E<T> const& x, E<U> const& y)
        { return hana::equal(x.value, y.value); }

        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<Either, Either> {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) apply(E<T> const& x, E<U> const& y)
        { return hana::less(x.value, y.value); }

        template <typename T, typename U>
        static constexpr auto apply(_left<T> const&, _right<U> const&)
        { return true_; }

        template <typename T, typename U>
        static constexpr auto apply(_right<T> const&, _left<U> const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<Either> {
        template <typename E, typename F>
        static constexpr decltype(auto) apply(E&& e, F&& f) {
            return hana::either(left,
                hana::compose(right, static_cast<F&&>(f)),
                static_cast<E&&>(e)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<Either> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::right(static_cast<X&&>(x)); }
    };

    template <>
    struct ap_impl<Either> {
        template <typename E, typename X>
        static constexpr decltype(auto) apply(E&& e, X&& x) {
            return hana::either(left,
                hana::partial(transform, static_cast<X&&>(x)),
                static_cast<E&&>(e)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<Either> {
        template <typename E>
        static constexpr decltype(auto) apply(E&& e)
        { return hana::either(left, id, static_cast<E&&>(e)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Either> {
        template <typename T, typename F>
        static constexpr decltype(auto) apply(_left<T> const&, F&& f)
        { return static_cast<F&&>(f)(); }


        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T> const& x, F&& f)
        { return static_cast<F&&>(f)(x.value); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>& x, F&& f)
        { return static_cast<F&&>(f)(x.value); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>&& x, F&& f)
        { return static_cast<F&&>(f)(detail::std::move(x.value)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_HPP
