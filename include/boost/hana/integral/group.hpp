/*!
@file
Defines the instance of `boost::hana::Group` for `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_GROUP_HPP
#define BOOST_HANA_INTEGRAL_GROUP_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/group/minus_mcd.hpp>
#include <boost/hana/integral/integral.hpp>

// Mcd
#include <boost/hana/integral/monoid.hpp>


namespace boost { namespace hana {
    //! Additive `Group` of `Integral`s.
    template <>
    struct Group::instance<Integral, Integral>
        : Group::minus_mcd<Integral, Integral>
    {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y)
        { return integral<decltype(value(x) - value(y)), value(x) - value(y)>; }
    };

    template <typename T>
    struct Group::instance<Integral, Foreign<T>,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Group::minus_mcd<Integral, Foreign<T>>
    {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y)
        { return value(x) - y; }
    };

    template <typename T>
    struct Group::instance<Foreign<T>, Integral,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Group::minus_mcd<Foreign<T>, Integral>
    {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y)
        { return x - value(y); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_GROUP_HPP
