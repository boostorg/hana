/*!
@file
Defines `boost::hana::Comparable::integral_constant_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_INTEGRAL_CONSTANT_MCD_HPP
#define BOOST_HANA_COMPARABLE_INTEGRAL_CONSTANT_MCD_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>
#include <boost/hana/logical/integral_constant_mcd.hpp>


namespace boost { namespace hana {
    template <typename I1, typename I2>
    struct Comparable::integral_constant_mcd : Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X x, Y y) {
            constexpr auto eq = value(x) == value(y);
            return integral_constant<I1, decltype(eq), eq>;
        }
    };

    template <typename I, typename T>
    struct Comparable::instance<I, T, when<
        is_an<IntegralConstant, I>() &&
        detail::std::is_integral<T>{}
    >>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X&& x, Y&& y) {
            return value(detail::std::forward<X>(x)) ==
                   detail::std::forward<Y>(y);
        }
    };

    template <typename T, typename I>
    struct Comparable::instance<T, I, when<
        detail::std::is_integral<T>{} &&
        is_an<IntegralConstant, I>()
    >>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) ==
                   value(detail::std::forward<Y>(y));
        }
    };

    template <typename I1, typename I2>
    struct Comparable::instance<I1, I2, when<
        is_an<IntegralConstant, I1>() &&
        is_an<IntegralConstant, I2>()
    >>
        : Comparable::integral_constant_mcd<I1, I2>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_INTEGRAL_CONSTANT_MCD_HPP
