/*!
@file
Defines `boost::hana::detail::right_folds::variadic_unrolled`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_RIGHT_FOLDS_VARIADIC_UNROLLED_HPP
#define BOOST_HANA_DETAIL_RIGHT_FOLDS_VARIADIC_UNROLLED_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana { namespace detail { namespace right_folds {
    template <typename F, typename State>
    auto variadic_unrolled_impl(F, State s)
    { return s; }

    template <typename F, typename State, typename X0>
    auto variadic_unrolled_impl(F f, State s, X0 x0)
    { return f(x0, s); }

    template <typename F, typename State, typename X0, typename X1>
    auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1)
    { return f(x0, f(x1, s)); }

    template <typename F, typename State, typename X0, typename X1, typename X2>
    auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1, X2 x2)
    { return f(x0, f(x1, f(x2, s))); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3>
    auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3)
    { return f(x0, f(x1, f(x2, f(x3, s)))); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3, typename X4>
    auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3, X4 x4)
    { return f(x0, f(x1, f(x2, f(x3, f(x4, s))))); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3, typename X4, typename X5, typename ...Xs>
    auto variadic_unrolled_impl(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3, X4 x4, X5 x5, Xs ...xs)
    { return f(x0, f(x1, f(x2, f(x3, f(x4, f(x5, variadic_unrolled_impl(f, s, xs...))))))); }

    BOOST_HANA_CONSTEXPR_LAMBDA auto variadic_unrolled = [](auto f, auto s, auto ...xs) {
        return variadic_unrolled_impl(f, s, xs...);
    };
}}}} // end namespace boost::hana::detail::right_folds

#endif // !BOOST_HANA_DETAIL_RIGHT_FOLDS_VARIADIC_UNROLLED_HPP
