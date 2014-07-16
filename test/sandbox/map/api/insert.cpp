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


template <int i> constexpr auto k = int_<i>;
template <int i> constexpr auto v = int_<-i>;

template <int i, int j>
BOOST_HANA_CONSTEXPR_LAMBDA auto p = pair(k<i>, v<j>);

int main() {
    BOOST_HANA_STATIC_ASSERT(insert(k<1>, v<1>, map()) == map(p<1, 1>));
    BOOST_HANA_STATIC_ASSERT(insert(k<1>, v<2>, map(p<1, 1>)) == map(p<1, 2>));
    BOOST_HANA_STATIC_ASSERT(insert(k<3>, v<3>, map(p<1, 1>, p<2, 2>)) == map(p<1,1>, p<2,2>, p<3,3>));
    BOOST_HANA_STATIC_ASSERT(insert(k<2>, v<-2>, map(p<1, 1>, p<2, 2>)) == map(p<1,1>, p<2,-2>));
}
