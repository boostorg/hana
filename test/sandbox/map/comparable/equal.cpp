/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/map.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/pair.hpp>
using namespace boost::hana;


template <int k, int v>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = pair(int_<k>, int_<v>);

int main() {
    BOOST_HANA_STATIC_ASSERT(equal(map(), map()));
    BOOST_HANA_STATIC_ASSERT(!equal(map(p<1, 1>), map()));
    BOOST_HANA_STATIC_ASSERT(!equal(map(), map(p<1, 1>)));

    BOOST_HANA_STATIC_ASSERT(equal(map(p<1, 1>), map(p<1, 1>)));
    BOOST_HANA_STATIC_ASSERT(!equal(map(p<1, 1>), map(p<1, 2>)));
    BOOST_HANA_STATIC_ASSERT(!equal(map(p<1, 1>), map(p<2, 1>)));
}
