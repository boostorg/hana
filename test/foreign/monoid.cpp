/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foreign/comparable.hpp>
#include <boost/hana/foreign/logical.hpp> // for laws
#include <boost/hana/foreign/monoid.hpp>

#include <boost/hana/detail/assert.hpp>

#define BOOST_HANA_TEST_MONOID
#include <test/builtin_integer.hpp>

#include <test/laws/monoid.hpp>
using namespace boost::hana;


int main() {
    constexpr int x = 6, y = 4;

    // zero
    {
        BOOST_HANA_CONSTEXPR_ASSERT(zero<integer>.value == 0);
    }

    // plus
    {
        // same type
        BOOST_HANA_CONSTEXPR_ASSERT(plus(integer{x}, integer{y}).value == x + y);

        // mixed types
        BOOST_HANA_CONSTEXPR_ASSERT(plus(integer{x}, integer2{y}).value == x + y);
        BOOST_HANA_CONSTEXPR_ASSERT(plus(integer2{x}, integer{y}).value == x + y);
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Monoid_laws<integer>(
            integer{0}, integer{1}, integer{2},
            integer2{0}, integer2{1}, integer2{2}
        ));
    }
}
