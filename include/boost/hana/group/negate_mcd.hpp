/*!
@file
Defines `boost::hana::Group::negate_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_NEGATE_MCD_HPP
#define BOOST_HANA_GROUP_NEGATE_MCD_HPP

#include <boost/hana/group/group.hpp>
#include <boost/hana/monoid/monoid.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `negate`
    template <typename G1, typename G2>
    struct Group::negate_mcd {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y) {
            return plus(x, negate(y));
        }
    };
}}

#endif // !BOOST_HANA_GROUP_NEGATE_MCD_HPP
