/*!
@file
Defines the instance of `boost::hana::Orderable` for `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_ORDERABLE_HPP
#define BOOST_HANA_INTEGRAL_ORDERABLE_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/integral/integral.hpp>
#include <boost/hana/orderable/less_mcd.hpp>


namespace boost { namespace hana {
    //! @details
    //! An `Integral` is less than another if and only if its underlying
    //! integral value is less than the other's.
    //!
    //! @snippet example/integral.cpp orderable
    template <>
    struct Orderable::instance<Integral, Integral> : Orderable::less_mcd {
        template <typename T, typename U>
        static constexpr auto less_impl(T t, U u) {
            return integral<
                decltype(t() < u()),
                (t() < u())
            >;
        }
    };

    //! `Integral`s can be ordered with `Foreign` objects of any arithmetic
    //! type.
    template <typename T>
    struct Orderable::instance<Integral, Foreign<T>,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr auto less_impl(X x, Y y)
        { return x() < y; }
    };

    template <typename T>
    struct Orderable::instance<Foreign<T>, Integral,
        when<detail::std::is_arithmetic<T>{}>
    >
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr auto less_impl(X x, Y y)
        { return x < y(); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_ORDERABLE_HPP
