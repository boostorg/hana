/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_STATIC_ASSERT( equal(nothing, nothing));
    BOOST_HANA_STATIC_ASSERT(!equal(nothing, just(x<0>)));
    BOOST_HANA_STATIC_ASSERT(!equal(just(x<0>), nothing));
    BOOST_HANA_STATIC_ASSERT( equal(just(x<0>), just(x<0>)));
    BOOST_HANA_STATIC_ASSERT(!equal(just(x<0>), just(x<1>)));
}
