/*!
@file
Defines `boost::hana::IntegralDomain::integral_constant_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_DOMAIN_INTEGRAL_CONSTANT_MCD_HPP
#define BOOST_HANA_INTEGRAL_DOMAIN_INTEGRAL_CONSTANT_MCD_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>
#include <boost/hana/integral_domain/mcd.hpp>


namespace boost { namespace hana {
    //! Any two `IntegralConstant`s form a multiplicative `IntegralDomain`.
    template <typename I1, typename I2>
    struct IntegralDomain::integral_constant_mcd : IntegralDomain::mcd {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y) {
            constexpr auto quotient = value(x) / value(y);
            return integral_constant<I1, decltype(quotient), quotient>;
        }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y) {
            constexpr auto modulus = value(x) % value(y);
            return integral_constant<I1, decltype(modulus), modulus>;
        }
    };

    template <typename I, typename T>
    struct IntegralDomain::instance<I, T, when<
        is_an<IntegralConstant, I>() &&
        detail::std::is_integral<T>{}
    >>
        : IntegralDomain::mcd
    {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y)
        { return value(x) / y; }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y)
        { return value(x) % y; }
    };

    template <typename T, typename I>
    struct IntegralDomain::instance<T, I, when<
        detail::std::is_integral<T>{} &&
        is_an<IntegralConstant, I>()
    >>
        : IntegralDomain::mcd
    {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y)
        { return x / value(y); }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y)
        { return x % value(y); }
    };

    template <typename I1, typename I2>
    struct IntegralDomain::instance<I1, I2, when<
        is_an<IntegralConstant, I1>() &&
        is_an<IntegralConstant, I2>()
    >>
        : IntegralDomain::integral_constant_mcd<I1, I2>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_INTEGRAL_CONSTANT_MCD_HPP
