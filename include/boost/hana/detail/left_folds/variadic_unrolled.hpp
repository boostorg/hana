/*!
@file
Defines `boost::hana::detail::left_folds::variadic_unrolled`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_UNROLLED_HPP
#define BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_UNROLLED_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana { namespace detail { namespace left_folds {
    template <typename F, typename State>
    constexpr auto variadic_unrolled_impl(F, State s)
    { return s; }

    template <typename F, typename State, typename X0>
    constexpr auto variadic_unrolled_impl(F f, State s, X0 x0)
    { return f(s, x0); }

    template <typename F, typename State, typename X0, typename X1>
    constexpr auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1)
    { return f(f(s, x0), x1); }

    template <typename F, typename State, typename X0, typename X1, typename X2>
    constexpr auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1, X2 x2)
    { return f(f(f(s, x0), x1), x2); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3>
    constexpr auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3)
    { return f(f(f(f(s, x0), x1), x2), x3); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3, typename X4>
    constexpr auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3, X4 x4)
    { return f(f(f(f(f(s, x0), x1), x2), x3), x4); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3, typename X4, typename X5, typename ...Xs>
    constexpr auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3, X4 x4, X5 x5, Xs ...xs)
    { return variadic_unrolled_impl(f, f(f(f(f(f(f(s, x0), x1), x2), x3), x4), x5), xs...); }

    BOOST_HANA_CONSTEXPR_LAMBDA auto variadic_unrolled = [](auto f, auto s, auto ...xs) {
        return variadic_unrolled_impl(f, s, xs...);
    };
}}}} // end namespace boost::hana::detail::left_folds

#endif // !BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_UNROLLED_HPP
