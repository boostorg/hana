/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


// We need the comparison to be compile-time.
template <int i>
constexpr auto x = int_<i>;

int main() {
    BOOST_HANA_STATIC_ASSERT(flatten(singleton(singleton(x<0>))) == singleton(x<0>));
    BOOST_HANA_STATIC_ASSERT(flatten(singleton(doubleton(x<0>, x<1>))) == doubleton(x<0>, x<1>));

    BOOST_HANA_STATIC_ASSERT(flatten(doubleton(singleton(x<0>), singleton(x<1>))) == doubleton(x<0>, x<1>));
    BOOST_HANA_STATIC_ASSERT(flatten(doubleton(doubleton(x<0>, x<1>), singleton(x<2>))) == union_(doubleton(x<0>, x<1>), singleton(x<2>)));
    BOOST_HANA_STATIC_ASSERT(flatten(doubleton(singleton(x<0>), doubleton(x<1>, x<2>))) == union_(doubleton(x<0>, x<1>), singleton(x<2>)));
    BOOST_HANA_STATIC_ASSERT(flatten(doubleton(doubleton(x<0>, x<1>), doubleton(x<2>, x<3>))) == union_(doubleton(x<0>, x<1>), doubleton(x<2>, x<3>)));
}
