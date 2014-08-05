/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/comparable/not_equal_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto comparable = detail::minimal::comparable<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(equal(comparable(0), comparable(0)));
    BOOST_HANA_CONSTEXPR_ASSERT(!(equal(comparable(0), comparable(1))));

    BOOST_HANA_CONSTEXPR_ASSERT(comparable(0) == comparable(0));
    BOOST_HANA_CONSTEXPR_ASSERT(!(comparable(0) == comparable(1)));
}

int main() {
    test<Comparable::equal_mcd>();
    test<Comparable::not_equal_mcd>();
}
