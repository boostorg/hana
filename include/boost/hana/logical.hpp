/*!
@file
Defines `boost::hana::Logical`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_HPP

#include <boost/hana/fwd/logical.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `and_`.
        //! @relates boost::hana::Logical
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value ||
            enable_operators<Logical, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator&&(X&& x, Y&& y) {
            return and_(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        //! Equivalent to `or_`.
        //! @relates boost::hana::Logical
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value ||
            enable_operators<Logical, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator||(X&& x, Y&& y) {
            return or_(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        //! Equivalent to `not_`.
        //! @relates boost::hana::Logical
        template <typename X, typename = typename detail::std::enable_if<
            enable_operators<Logical, datatype_t<X>>::value
        >::type>
        constexpr decltype(auto) operator!(X&& x) {
            return not_(detail::std::forward<X>(x));
        }
    }

    // Minimal complete definitions
    // ----------------------------
    // 1. `eval_if`, `not_` and `while_`
    template <typename L, typename _>
    struct or_impl<L, when<
        is_implemented<eval_if_impl<L>, _> &&
        is_implemented<not_impl<L>, _> &&
        is_implemented<while_impl<L>, _>
    >, _> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            //! @todo How to forward `x` here? Since the arguments to `if_` can be
            //! evaluated in any order, we have to be careful not to use `x` in
            //! a moved-from state.
            return if_(x, x, detail::std::forward<Y>(y));
        }
    };

    template <typename L, typename _>
    struct and_impl<L, when<
        is_implemented<eval_if_impl<L>, _> &&
        is_implemented<not_impl<L>, _> &&
        is_implemented<while_impl<L>, _>
    >, _> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return if_(x, detail::std::forward<Y>(y), x);
        }
    };

    template <typename L, typename _>
    struct if_impl<L, when<
        is_implemented<eval_if_impl<L>, _> &&
        is_implemented<not_impl<L>, _> &&
        is_implemented<while_impl<L>, _>
    >, _> {
        //! @todo By using `always` here, we create a copy of both `t` and `e`,
        //! which is not very smart.
        template <typename C, typename T, typename E>
        static constexpr decltype(auto) apply(C&& c, T&& t, E&& e) {
            return eval_if(detail::std::forward<C>(c),
                always(detail::std::forward<T>(t)),
                always(detail::std::forward<E>(e))
            );
        }
    };

    template <typename L, typename _>
    struct until_impl<L, when<
        is_implemented<eval_if_impl<L>, _> &&
        is_implemented<not_impl<L>, _> &&
        is_implemented<while_impl<L>, _>
    >, _> {
        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto) apply(Pred&& pred, State&& state, F&& f) {
            return while_(compose(not_, detail::std::forward<Pred>(pred)),
                          detail::std::forward<State>(state),
                          detail::std::forward<F>(f));
        }
    };


    // 2. Data types implicitly convertible to `bool`
    template <typename L>
    struct eval_if_impl<L, when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >> {
        template <typename T, typename E>
        static constexpr auto apply(bool cond, T t, E e)
        { return cond ? t(id) : e(id); }
    };

    template <typename L>
    struct not_impl<L, when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >> {
        static constexpr bool apply(bool cond)
        { return !cond; }
    };

    template <typename L>
    struct while_impl<L, when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >> {
        template <typename Pred, typename State, typename F>
        static auto apply(Pred&& pred, State&& state, F&& f)
            -> decltype(
                true ? f(detail::std::forward<State>(state))
                     : detail::std::forward<State>(state)
            )
        {
            if (pred(state)) {
                decltype(auto) r = f(detail::std::forward<State>(state));
                return while_(detail::std::forward<Pred>(pred),
                              detail::std::forward<decltype(r)>(r),
                              detail::std::forward<F>(f));
            }
            else {
                return detail::std::forward<State>(state);
            }
        }
    };

    template <typename L>
    constexpr auto is_a<Logical, L> = bool_<
        is_implemented<eval_if_impl<L>> &&
        is_implemented<not_impl<L>> &&
        is_implemented<while_impl<L>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_HPP
