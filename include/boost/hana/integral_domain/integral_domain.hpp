/*!
@file
Forward declares `boost::hana::IntegralDomain`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_DOMAIN_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_INTEGRAL_DOMAIN_INTEGRAL_DOMAIN_HPP

#include <boost/hana/integral_domain/detail/integral_domain_fwd.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/ring/ring.hpp>


namespace boost { namespace hana {
    //! Instance of `IntegralDomain` for objects with numeric data types.
    //!
    //! Any two objects whose data types are `Rings`s and can be divided and
    //! moded with the usual operators `/` and `%` naturally form an integral
    //! domain with those operations.
    template <typename X, typename Y>
    struct IntegralDomain::instance<X, Y, when<
        are<Ring, X, Y>() &&
        is_valid<decltype((void)(
            *(X*)1 % *(Y*)1,
            *(X*)1 / *(Y*)1
        ))>
    >> : IntegralDomain::mcd {
        static constexpr auto quot_impl(X x, Y y)
        { return x / y; }

        static constexpr auto mod_impl(X x, Y y)
        { return x % y; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_INTEGRAL_DOMAIN_HPP
