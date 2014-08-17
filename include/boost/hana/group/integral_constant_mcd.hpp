/*!
@file
Defines `boost::hana::Group::integral_constant_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_INTEGRAL_CONSTANT_MCD_HPP
#define BOOST_HANA_GROUP_INTEGRAL_CONSTANT_MCD_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/group/minus_mcd.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>


namespace boost { namespace hana {
    //! Any two `IntegralConstant`s form an additive `Group`.
    template <typename I1, typename I2>
    struct Group::integral_constant_mcd : Group::minus_mcd<I1, I2> {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y) {
            constexpr auto sub = value(x) - value(y);
            return integral_constant<I1, decltype(sub), sub>;
        }
    };

    template <typename I, typename T>
    struct Group::instance<I, Foreign<T>, when<
        is_an<IntegralConstant, I>() &&
        detail::std::is_integral<T>{}
    >>
        : Group::minus_mcd<I, Foreign<T>>
    {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y)
        { return value(x) - y; }
    };

    template <typename T, typename I>
    struct Group::instance<Foreign<T>, I, when<
        detail::std::is_integral<T>{} &&
        is_an<IntegralConstant, I>()
    >>
        : Group::minus_mcd<Foreign<T>, I>
    {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y)
        { return x - value(y); }
    };

    template <typename I1, typename I2>
    struct Group::instance<I1, I2, when<
        is_an<IntegralConstant, I1>() &&
        is_an<IntegralConstant, I2>()
    >>
        : Group::integral_constant_mcd<I1, I2>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_INTEGRAL_CONSTANT_MCD_HPP
