/*!
@file
Defines `boost::hana::IntegralDomain::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_DOMAIN_MCD_HPP
#define BOOST_HANA_INTEGRAL_DOMAIN_MCD_HPP

#include <boost/hana/integral_domain/integral_domain.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `quot` and `mod`
    struct IntegralDomain::mcd { };
}}

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_MCD_HPP
