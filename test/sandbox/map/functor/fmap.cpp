/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/map.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/pair/instance.hpp>
using namespace boost::hana;


template <int k, int v>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = pair(int_<k>, int_<v>);

BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) { return x + int_<1>; };

int main() {
    BOOST_HANA_STATIC_ASSERT(fmap(inc, map()) == map());
    BOOST_HANA_STATIC_ASSERT(fmap(inc, map(p<1, 1>)) == map(p<1, 2>));
    BOOST_HANA_STATIC_ASSERT(fmap(inc, map(p<1, 1>, p<2, 2>)) == map(p<1, 2>, p<2, 3>));
    BOOST_HANA_STATIC_ASSERT(fmap(inc, map(p<1, 1>, p<2, 2>, p<3, 3>)) == map(p<1, 2>, p<2, 3>, p<3, 4>));
}
