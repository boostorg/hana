/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


// search needs the comparison to be compile-time.
template <int i>
constexpr auto x = int_<i>;

BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

int main() {
    BOOST_HANA_STATIC_ASSERT(search(singleton(x<0>), is(x<0>)) == just(x<0>));
    BOOST_HANA_STATIC_ASSERT(search(singleton(x<1>), is(x<0>)) == nothing);

    BOOST_HANA_STATIC_ASSERT(search(doubleton(x<0>, x<1>), is(x<0>)) == just(x<0>));
    BOOST_HANA_STATIC_ASSERT(search(doubleton(x<0>, x<1>), is(x<1>)) == just(x<1>));
    BOOST_HANA_STATIC_ASSERT(search(doubleton(x<0>, x<1>), is(x<2>)) == nothing);
}
