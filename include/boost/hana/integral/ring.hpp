/*!
@file
Defines the instance of `boost::hana::Ring` for `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_RING_HPP
#define BOOST_HANA_INTEGRAL_RING_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/integral/integral.hpp>
#include <boost/hana/ring/mcd.hpp>

// Mcd
#include <boost/hana/integral/group.hpp>


namespace boost { namespace hana {
    //! Multiplicative `Ring` of `Integral`s.
    template <>
    struct Ring::instance<Integral, Integral>
        : Ring::mcd
    {
        template <typename X, typename Y>
        static constexpr auto mult_impl(X x, Y y) {
            return integral<
                decltype(value(x) * value(y)),
                value(x) * value(y)
            >;
        }

        static constexpr auto one_impl()
        { return int_<1>; }
    };

    template <typename T>
    struct Ring::instance<Integral, Foreign<T>,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Ring::mcd
    {
        template <typename X, typename Y>
        static constexpr auto mult_impl(X x, Y y)
        { return value(x) * y; }
    };

    template <typename T>
    struct Ring::instance<Foreign<T>, Integral,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Ring::mcd
    {
        template <typename X, typename Y>
        static constexpr auto mult_impl(X x, Y y)
        { return x * value(y); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_RING_HPP
