/*!
@file
Defines the instance of `boost::hana::IntegralDomain` for `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_INTEGRAL_INTEGRAL_DOMAIN_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/integral/integral.hpp>
#include <boost/hana/integral_domain/mcd.hpp>

// Mcd
#include <boost/hana/integral/ring.hpp>


namespace boost { namespace hana {
    //! Integral domain of `Integral`s.
    template <>
    struct IntegralDomain::instance<Integral, Integral>
        : IntegralDomain::mcd
    {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y) {
            return integral<
                decltype(value(x) / value(y)),
                value(x) / value(y)
            >;
        }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y) {
            return integral<
                decltype(value(x) % value(y)),
                value(x) % value(y)
            >;
        }
    };

    template <typename T>
    struct IntegralDomain::instance<Integral, Foreign<T>,
        when<detail::std::is_arithmetic<T>{}>
    >
        : IntegralDomain::mcd
    {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y)
        { return value(x) / y; }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y)
        { return value(x) % y; }
    };

    template <typename T>
    struct IntegralDomain::instance<Foreign<T>, Integral,
        when<detail::std::is_arithmetic<T>{}>
    >
        : IntegralDomain::mcd
    {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y)
        { return x / value(y); }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y)
        { return x % value(y); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_INTEGRAL_DOMAIN_HPP
