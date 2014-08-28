/*!
@file
Defines `boost::hana::detail::variadic::reverse_apply`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_HPP
#define BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_HPP

namespace boost { namespace hana { namespace detail { namespace variadic {
    template <typename F>
    constexpr auto reverse_apply(F f)
    { return f(); }

    template <typename F, typename X1>
    constexpr auto reverse_apply(F f, X1 x1)
    { return f(x1); }

    template <typename F, typename X1, typename X2>
    constexpr auto reverse_apply(F f, X1 x1, X2 x2)
    { return f(x2, x1); }

    template <typename F, typename X1, typename X2, typename X3>
    constexpr auto reverse_apply(F f, X1 x1, X2 x2, X3 x3)
    { return f(x3, x2, x1); }

    template <typename F, typename X1, typename X2, typename X3, typename X4>
    constexpr auto reverse_apply(F f, X1 x1, X2 x2, X3 x3, X4 x4)
    { return f(x4, x3, x2, x1); }

    template <typename F, typename X1, typename X2, typename X3, typename X4, typename X5>
    constexpr auto reverse_apply(F f, X1 x1, X2 x2, X3 x3, X4 x4, X5 x5)
    { return f(x5, x4, x3, x2, x1); }

    template <typename F, typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename ...Xn>
    constexpr auto reverse_apply(F f, X1 x1, X2 x2, X3 x3, X4 x4, X5 x5, X6 x6, Xn ...xn) {
        return reverse_apply([=](auto ...xn) {
            return f(xn..., x6, x5, x4, x3, x2, x1);
        }, xn...);
    }
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_HPP
