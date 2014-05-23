/*!
 * @file
 * Defines `boost::hana::detail::left_folds::variadic_unrolled`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_UNROLLED_HPP
#define BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_UNROLLED_HPP

namespace boost { namespace hana { namespace detail { namespace left_folds {
    template <typename F, typename State>
    auto variadic_unrolled(F, State s)
    { return s; }

    template <typename F, typename State, typename X0>
    auto variadic_unrolled(F f, State s, X0 x0)
    { return f(s, x0); }

    template <typename F, typename State, typename X0, typename X1>
    auto variadic_unrolled(F f, State s, X0 x0, X1 x1)
    { return f(f(s, x0), x1); }

    template <typename F, typename State, typename X0, typename X1, typename X2>
    auto variadic_unrolled(F f, State s, X0 x0, X1 x1, X2 x2)
    { return f(f(f(s, x0), x1), x2); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3>
    auto variadic_unrolled(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3)
    { return f(f(f(f(s, x0), x1), x2), x3); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3, typename X4>
    auto variadic_unrolled(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3, X4 x4)
    { return f(f(f(f(f(s, x0), x1), x2), x3), x4); }

    template <typename F, typename State, typename X0, typename X1, typename X2, typename X3, typename X4, typename X5, typename ...Xs>
    auto variadic_unrolled(F f, State s, X0 x0, X1 x1, X2 x2, X3 x3, X4 x4, X5 x5, Xs ...xs)
    { return variadic_unrolled(f, f(f(f(f(f(f(s, x0), x1), x2), x3), x4), x5), xs...); }
}}}} // end namespace boost::hana::detail::left_folds

#endif // !BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_UNROLLED_HPP
