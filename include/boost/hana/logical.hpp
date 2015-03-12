/*!
@file
Defines `boost::hana::Logical`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_HPP

#include <boost/hana/fwd/logical.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            has_operator<datatype_t<X>, decltype(and_)>::value ||
            has_operator<datatype_t<Y>, decltype(and_)>::value
        >::type>
        constexpr decltype(auto) operator&&(X&& x, Y&& y) {
            return hana::and_(detail::std::forward<X>(x),
                              detail::std::forward<Y>(y));
        }

        template <typename X, typename Y, typename = typename detail::std::enable_if<
            has_operator<datatype_t<X>, decltype(or_)>::value ||
            has_operator<datatype_t<Y>, decltype(or_)>::value
        >::type>
        constexpr decltype(auto) operator||(X&& x, Y&& y) {
            return hana::or_(detail::std::forward<X>(x),
                             detail::std::forward<Y>(y));
        }

        template <typename X, typename = typename detail::std::enable_if<
            has_operator<datatype_t<X>, decltype(not_)>::value
        >::type>
        constexpr decltype(auto) operator!(X&& x) {
            return hana::not_(detail::std::forward<X>(x));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // if_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct if_impl : if_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct if_impl<L, when<condition>> : default_ {
        //! @todo By using `always` here, we create a copy of both `t` and `e`,
        //! which is not very smart.
        template <typename C, typename T, typename E>
        static constexpr decltype(auto) apply(C&& c, T&& t, E&& e) {
            return hana::eval_if(detail::std::forward<C>(c),
                hana::always(detail::std::forward<T>(t)),
                hana::always(detail::std::forward<E>(e))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // eval_if
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct eval_if_impl : eval_if_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct eval_if_impl<L, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // while_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct while_impl : while_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct while_impl<L, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // until
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct until_impl : until_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct until_impl<L, when<condition>> : default_ {
        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto) apply(Pred&& pred, State&& state, F&& f) {
            return hana::while_(
                    hana::compose(not_, detail::std::forward<Pred>(pred)),
                    detail::std::forward<State>(state),
                    detail::std::forward<F>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // not_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct not_impl : not_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct not_impl<L, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // and_
    //////////////////////////////////////////////////////////////////////////
    // Remove warnings generated by poor confused Doxygen
    //! @cond
    template <typename X, typename Y>
    constexpr decltype(auto) _and::operator()(X&& x, Y&& y) const {
        return and_impl<typename datatype<X>::type>::apply(
            detail::std::forward<X>(x),
            detail::std::forward<Y>(y)
        );
    }

    template <typename X, typename ...Y>
    constexpr decltype(auto) _and::operator()(X&& x, Y&& ...y) const {
        return detail::variadic::foldl(
            *this,
            detail::std::forward<X>(x),
            detail::std::forward<Y>(y)...
        );
    }
    //! @endcond

    template <typename L, typename>
    struct and_impl : and_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct and_impl<L, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::if_(x, detail::std::forward<Y>(y), x);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // or_
    //////////////////////////////////////////////////////////////////////////
    // Remove warnings generated by poor confused Doxygen
    //! @cond
    template <typename X, typename Y>
    constexpr decltype(auto) _or::operator()(X&& x, Y&& y) const {
        return or_impl<typename datatype<X>::type>::apply(
            detail::std::forward<X>(x),
            detail::std::forward<Y>(y)
        );
    }

    template <typename X, typename ...Y>
    constexpr decltype(auto) _or::operator()(X&& x, Y&& ...y) const {
        return detail::variadic::foldl(
            *this,
            detail::std::forward<X>(x),
            detail::std::forward<Y>(y)...
        );
    }
    //! @endcond

    template <typename L, typename>
    struct or_impl : or_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct or_impl<L, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            //! @todo How to forward `x` here? Since the arguments to `if_`
            //! can be evaluated in any order, we have to be careful not to
            //! use `x` in a moved-from state.
            return hana::if_(x, x, detail::std::forward<Y>(y));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename L>
    struct models<Logical, L>
        : detail::std::integral_constant<bool,
            !is_default<eval_if_impl<L>>{} &&
            !is_default<not_impl<L>>{} &&
            !is_default<while_impl<L>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename L>
    struct eval_if_impl<L, when<detail::std::is_arithmetic<L>{}>> {
        template <typename Cond, typename T, typename E>
        static constexpr auto apply(Cond cond, T t, E e)
        { return cond ? t(id) : e(id); }
    };

    template <typename L>
    struct not_impl<L, when<detail::std::is_arithmetic<L>{}>> {
        template <typename Cond>
        static constexpr Cond apply(Cond cond)
        { return static_cast<Cond>(cond ? false : true); }
    };

    template <typename L>
    struct while_impl<L, when<detail::std::is_arithmetic<L>{}>> {
        template <typename Pred, typename State, typename F>
        static auto apply(Pred&& pred, State&& state, F&& f)
            -> decltype(
                true ? f(detail::std::forward<State>(state))
                     : detail::std::forward<State>(state)
            )
        {
            if (pred(state)) {
                decltype(auto) r = f(detail::std::forward<State>(state));
                return hana::while_(detail::std::forward<Pred>(pred),
                                    detail::std::forward<decltype(r)>(r),
                                    detail::std::forward<F>(f));
            }
            else {
                return detail::std::forward<State>(state);
            }
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Logical
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct eval_if_impl<C, when<
        models<Constant, C>{} && models<Logical, typename C::value_type>{}
    >> {
        template <typename Then, typename Else>
        static constexpr auto
        eval_if_helper(detail::std::true_type, Then t, Else e)
        { return t(id); }

        template <typename Then, typename Else>
        static constexpr auto
        eval_if_helper(detail::std::false_type, Then t, Else e)
        { return e(id); }

        template <typename Cond, typename Then, typename Else>
        static constexpr auto apply(Cond const&, Then t, Else e) {
            constexpr auto cond = boost::hana::value<Cond>();
            constexpr bool truth_value = hana::if_(cond, true, false);
            return eval_if_helper(
                    detail::std::integral_constant<bool, truth_value>{}, t, e);
        }
    };

    template <typename C>
    struct not_impl<C, when<
        models<Constant, C>{} && models<Logical, typename C::value_type>{}
    >> {
        using T = typename C::value_type;
        template <typename Cond>
        struct _constant {
            static constexpr decltype(auto) get()
            { return boost::hana::not_(boost::hana::value<Cond>()); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename Cond>
        static constexpr auto apply(Cond const&)
        { return to<C>(_constant<Cond>{}); }
    };

    template <typename C>
    struct while_impl<C, when<
        models<Constant, C>{} && models<Logical, typename C::value_type>{}
    >> {
        template <typename Pred, typename State, typename F>
        static constexpr State
        while_helper(detail::std::false_type, Pred&& pred, State&& state, F&& f) {
            return detail::std::forward<State>(state);
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        while_helper(detail::std::true_type, Pred&& pred, State&& state, F&& f) {
            decltype(auto) r = f(detail::std::forward<State>(state));
            return hana::while_(detail::std::forward<Pred>(pred),
                                detail::std::forward<decltype(r)>(r),
                                detail::std::forward<F>(f));
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        apply(Pred&& pred, State&& state, F&& f) {
            // Since `pred(state)` returns a `Constant`, we do not actually
            // need to call it; we only need its decltype. However, we still
            // call it to run potential side effects. I'm not sure whether
            // that is desirable, since we pretty much take for granted that
            // functions are pure, but we'll do it like this for now. Also, I
            // think there is something rather deep hidden behind this, and
            // understanding what must be done here should give us a better
            // understanding of something non-trivial.
            auto cond_ = pred(state);
            constexpr auto cond = hana::value(cond_);
            constexpr bool truth_value = hana::if_(cond, true, false);
            return while_helper(detail::std::integral_constant<bool, truth_value>{},
                                detail::std::forward<Pred>(pred),
                                detail::std::forward<State>(state),
                                detail::std::forward<F>(f));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_HPP
