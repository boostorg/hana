/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto x, auto y) { return x + y; };
BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) { return x + 1; };

int main() {
    BOOST_HANA_CONSTANT_ASSERT(zip_with(inc, list()) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(inc, list(0)) == list(1));
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(inc, list(0, 1)) == list(1, 2));
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(inc, list(0, 1, 2)) == list(1, 2, 3));
    BOOST_HANA_CONSTANT_ASSERT(zip_with(inc, list(), list()) == list());

    BOOST_HANA_CONSTANT_ASSERT(zip_with(plus, list()) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(plus, list(), list()) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(plus, list(1), list()) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(plus, list(), list(3)) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(plus, list(1), list(3)) == list(1 + 3));
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(plus, list(1, 2), list(3, 4)) == list(1 + 3, 2 + 4));
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(plus, list(1, 2, 3, 4), list(5, 6, 7)) == list(1 + 5, 2 + 6, 3 + 7));
    BOOST_HANA_CONSTANT_ASSERT(zip_with(plus, list(), list(), list()) == list());
}
