/*!
@file
Defines `boost::hana::Monoid::integral_constant_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_INTEGRAL_CONSTANT_MCD_HPP
#define BOOST_HANA_MONOID_INTEGRAL_CONSTANT_MCD_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>
#include <boost/hana/monoid/mcd.hpp>


namespace boost { namespace hana {
    //! Any two `IntegralConstant`s form an additive `Monoid`.
    template <typename I1, typename I2>
    struct Monoid::integral_constant_mcd : Monoid::mcd {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y) {
            constexpr auto sum = value(x) + value(y);
            return integral_constant<I1, decltype(sum), sum>;
        }

        static BOOST_HANA_CONSTEXPR_LAMBDA auto zero_impl()
        { return integral_constant<I1, int, 0>; }
    };

    template <typename I, typename T>
    struct Monoid::instance<I, Foreign<T>, when<
        is_an<IntegralConstant, I>() &&
        detail::std::is_integral<T>{}
    >>
        : Monoid::mcd
    {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y)
        { return value(x) + y; }
    };

    template <typename T, typename I>
    struct Monoid::instance<Foreign<T>, I, when<
        detail::std::is_integral<T>{} &&
        is_an<IntegralConstant, I>()
    >>
        : Monoid::mcd
    {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y)
        { return x + value(y); }
    };

    template <typename I1, typename I2>
    struct Monoid::instance<I1, I2, when<
        is_an<IntegralConstant, I1>() &&
        is_an<IntegralConstant, I2>()
    >>
        : Monoid::integral_constant_mcd<I1, I2>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_INTEGRAL_CONSTANT_MCD_HPP
