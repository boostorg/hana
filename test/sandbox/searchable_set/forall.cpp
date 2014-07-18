/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

BOOST_HANA_CONSTEXPR_LAMBDA auto isnt = [](auto x) {
    return [=](auto y) { return not_equal(x, y); };
};

int main() {
    BOOST_HANA_STATIC_ASSERT(!forall(singleton(x<0>), isnt(x<0>)));
    BOOST_HANA_STATIC_ASSERT( forall(singleton(x<0>), isnt(x<1>)));
    BOOST_HANA_STATIC_ASSERT(!forall(doubleton(x<0>, x<1>), isnt(x<0>)));
    BOOST_HANA_STATIC_ASSERT(!forall(doubleton(x<0>, x<1>), isnt(x<1>)));
    BOOST_HANA_STATIC_ASSERT( forall(doubleton(x<0>, x<1>), isnt(x<2>)));
}
