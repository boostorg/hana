/*!
@file
Defines the instance of `boost::hana::Monoid` for `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_MONOID_HPP
#define BOOST_HANA_INTEGRAL_MONOID_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/integral/integral.hpp>
#include <boost/hana/monoid/mcd.hpp>


namespace boost { namespace hana {
    //! Additive `Monoid` of `Integral`s.
    template <>
    struct Monoid::instance<Integral, Integral> : Monoid::mcd {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y) {
            return integral<
                decltype(value(x) + value(y)),
                value(x) + value(y)
            >;
        }

        static constexpr auto zero_impl()
        { return int_<0>; }
    };

    template <typename T>
    struct Monoid::instance<Integral, Foreign<T>,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Monoid::mcd
    {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y)
        { return value(x) + y; }
    };

    template <typename T>
    struct Monoid::instance<Foreign<T>, Integral,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Monoid::mcd
    {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y)
        { return x + value(y); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_MONOID_HPP
