/*!
@file
Defines `boost::hana::Group::minus_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_MINUS_MCD_HPP
#define BOOST_HANA_GROUP_MINUS_MCD_HPP

#include <boost/hana/group/group.hpp>
#include <boost/hana/monoid/monoid.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `minus`
    template <typename G1, typename G2>
    struct Group::minus_mcd {
        template <typename X>
        static constexpr auto negate_impl(X x) {
            return minus(zero<G1>, x);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_MINUS_MCD_HPP
