/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monoid/monoid.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/assert.hpp>

#define BOOST_HANA_TEST_MONOID
#include "integer.hpp"
using namespace boost::hana;


using Integer = datatype_t<integer>;
using Integer2 = datatype_t<integer2>;

int main() {
    // same type
    BOOST_HANA_CONSTEXPR_ASSERT(zero<Integer>.value == 0);
    BOOST_HANA_CONSTEXPR_ASSERT(plus(integer{3}, integer{5}).value == 3 + 5);

    BOOST_HANA_CONSTEXPR_ASSERT(zero<Integer2>.value == 0);
    BOOST_HANA_CONSTEXPR_ASSERT(plus(integer2{3}, integer2{5}).value == 3 + 5);

    // mixed types
    BOOST_HANA_CONSTEXPR_ASSERT(plus(integer{3}, integer2{5}).value == 3 + 5);
    BOOST_HANA_CONSTEXPR_ASSERT(plus(integer2{3}, integer{5}).value == 3 + 5);
}
