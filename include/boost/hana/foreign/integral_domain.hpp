/*!
@file
Defines the instance of `boost::hana::IntegralDomain` for `boost::hana::Foreign`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_FOREIGN_INTEGRAL_DOMAIN_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/integral_domain/mcd.hpp>

// Mcd
#include <boost/hana/foreign/ring.hpp>


namespace boost { namespace hana {
    //! Instance of `IntegralDomain` for `Foreign` objects with numeric types.
    //!
    //! Any two `Foreign` objects that are `Rings`s, that can be divided
    //! and moded with the usual operators (`/` and `%`) naturally form
    //! an integral domain with those operations.
    template <typename T, typename U>
    struct IntegralDomain::instance<Foreign<T>, Foreign<U>, when<
        are<Ring, Foreign<T>, Foreign<U>>() &&
        is_valid<decltype((void)(
            *(T*)1 % *(U*)1,
            *(T*)1 / *(U*)1
        ))>
    >> : IntegralDomain::mcd {
        template <typename X, typename Y>
        static constexpr auto quot_impl(X x, Y y)
        { return x / y; }

        template <typename X, typename Y>
        static constexpr auto mod_impl(X x, Y y)
        { return x % y; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_INTEGRAL_DOMAIN_HPP
