/*!
@file
Defines `boost::hana::detail::variadic::reverse_apply_unrolled`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_UNROLLED_HPP
#define BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_UNROLLED_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <typename F>
    constexpr decltype(auto) reverse_apply_unrolled(F&& f) {
        return detail::std::forward<F>(f)();
    }

    template <typename F, typename X1>
    constexpr decltype(auto) reverse_apply_unrolled(F&& f, X1&& x1) {
        return detail::std::forward<F>(f)(
            detail::std::forward<X1>(x1)
        );
    }

    template <typename F, typename X1, typename X2>
    constexpr decltype(auto) reverse_apply_unrolled(F&& f, X1&& x1, X2&& x2) {
        return detail::std::forward<F>(f)(
            detail::std::forward<X2>(x2),
            detail::std::forward<X1>(x1)
        );
    }

    template <typename F, typename X1, typename X2, typename X3>
    constexpr decltype(auto) reverse_apply_unrolled(F&& f, X1&& x1, X2&& x2, X3&& x3) {
        return detail::std::forward<F>(f)(
            detail::std::forward<X3>(x3),
            detail::std::forward<X2>(x2),
            detail::std::forward<X1>(x1)
        );
    }

    template <typename F, typename X1, typename X2, typename X3, typename X4>
    constexpr decltype(auto) reverse_apply_unrolled(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4) {
        return detail::std::forward<F>(f)(
            detail::std::forward<X4>(x4),
            detail::std::forward<X3>(x3),
            detail::std::forward<X2>(x2),
            detail::std::forward<X1>(x1)
        );
    }

    template <typename F, typename X1, typename X2, typename X3, typename X4, typename X5>
    constexpr decltype(auto) reverse_apply_unrolled(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, X5&& x5) {
        return detail::std::forward<F>(f)(
            detail::std::forward<X5>(x5),
            detail::std::forward<X4>(x4),
            detail::std::forward<X3>(x3),
            detail::std::forward<X2>(x2),
            detail::std::forward<X1>(x1)
        );
    }

    template <typename F, typename X1, typename X2, typename X3, typename X4, typename X5, typename X6, typename ...Xn>
    constexpr decltype(auto) reverse_apply_unrolled(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, X5&& x5, X6 x6, Xn&& ...xn) {
        return reverse_apply_unrolled([
            f(detail::std::forward<F>(f)),
            x1(detail::std::forward<X1>(x1)),
            x2(detail::std::forward<X2>(x2)),
            x3(detail::std::forward<X3>(x3)),
            x4(detail::std::forward<X4>(x4)),
            x5(detail::std::forward<X5>(x5)),
            x6(detail::std::forward<X6>(x6))
        ](auto&& ...xn) -> decltype(auto) {
            return detail::std::move(f)(
                detail::std::forward<decltype(xn)>(xn)...,
                detail::std::move(x6),
                detail::std::move(x5),
                detail::std::move(x4),
                detail::std::move(x3),
                detail::std::move(x2),
                detail::std::move(x1)
            );
        }, detail::std::forward<Xn>(xn)...);
    }
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_REVERSE_APPLY_UNROLLED_HPP
