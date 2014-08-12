/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_COMPARABLE_HPP
#define BOOST_HANA_INTEGRAL_COMPARABLE_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/integral/integral.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Integral`s.
    //!
    //! Two `Integral`s are equal if and only if their underlying values
    //! can be compared and are equal.
    //!
    //! ### Example
    //! @snippet example/integral.cpp comparable
    //!
    //! @todo
    //! It does not really make sense for incomparable `Integral`s to be
    //! considered unequal, since `Integral`s should only contain values of
    //! integral types, which are all comparable.
    template <>
    struct Comparable::instance<Integral, Integral> : Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return integral<
                decltype(value(x) == value(y)),
                value(x) == value(y)
            >;
        }

        static constexpr auto equal_impl(...)
        { return integral<bool, false>; }
    };

    //! `Integral`s can be compared with `Foreign` objects of any
    //! arithmetic type.
    template <typename T>
    struct Comparable::instance<Integral, Foreign<T>,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return x() == y; }
    };

    template <typename T>
    struct Comparable::instance<Foreign<T>, Integral,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return x == y(); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_COMPARABLE_HPP
