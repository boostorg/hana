/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include "udt.hpp"
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(to<Map>(udt{0, '0'}) == map(
        pair(member1, 0),
        pair(member2, '0')
    ));

    BOOST_HANA_CONSTEXPR_ASSERT(to<Map>(udt{3, '5'}) == map(
        pair(member1, 3),
        pair(member2, '5')
    ));
}
