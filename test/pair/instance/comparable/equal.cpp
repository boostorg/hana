/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/pair/instance.hpp>

#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_STATIC_ASSERT( equal(pair(x<1>, x<2>), pair(x<1>, x<2>)));
    BOOST_HANA_STATIC_ASSERT(!equal(pair(x<1>, x<2>), pair(x<1>, x<999>)));
    BOOST_HANA_STATIC_ASSERT(!equal(pair(x<1>, x<2>), pair(x<999>, x<2>)));
    BOOST_HANA_STATIC_ASSERT(!equal(pair(x<1>, x<2>), pair(x<888>, x<999>)));
}
