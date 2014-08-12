/*!
@file
Defines `boost::hana::Monoid::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_MCD_HPP
#define BOOST_HANA_MONOID_MCD_HPP

#include <boost/hana/monoid/monoid.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `zero` and `plus`
    struct Monoid::mcd { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_MCD_HPP
