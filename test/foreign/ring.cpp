/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foreign.hpp>

#include <boost/hana/detail/assert.hpp>

#define BOOST_HANA_TEST_RING
#include <test/builtin_integer.hpp>

#include <test/laws/ring.hpp>
using namespace boost::hana;


int main() {
    constexpr int x = 6, y = 4;

    // one
    {
        BOOST_HANA_CONSTEXPR_ASSERT(one<integer>.value == 1);
    }

    // mult
    {
        // same type
        BOOST_HANA_CONSTEXPR_ASSERT(mult(integer{x}, integer{y}).value == x * y);

        // mixed types
        BOOST_HANA_CONSTEXPR_ASSERT(mult(integer{x}, integer2{y}).value == x * y);
        BOOST_HANA_CONSTEXPR_ASSERT(mult(integer2{x}, integer{y}).value == x * y);
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Ring_laws<integer>(
            integer{0}, integer{1}, integer{2},
            integer2{0}, integer2{1}, integer2{2}
        ));
    }
}
