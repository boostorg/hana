/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foreign/comparable.hpp>
#include <boost/hana/foreign/group.hpp>
#include <boost/hana/foreign/logical.hpp> // for laws

#include <boost/hana/detail/assert.hpp>

#define BOOST_HANA_TEST_GROUP
#include <test/builtin_integer.hpp>

#include <test/laws/group.hpp>
using namespace boost::hana;


int main() {
    // negate
    {
        BOOST_HANA_CONSTEXPR_ASSERT(negate(integer{1}).value == -1);
    }

    // minus
    {
        // same type
        BOOST_HANA_CONSTEXPR_ASSERT(minus(integer{1}, integer{3}).value == 1 - 3);

        // mixed types
        BOOST_HANA_CONSTEXPR_ASSERT(minus(integer{1}, integer2{3}).value == 1 - 3);
        BOOST_HANA_CONSTEXPR_ASSERT(minus(integer2{1}, integer{3}).value == 1 - 3);
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Group_laws<integer>(
            integer{0}, integer{1}, integer{2},
            integer2{0}, integer2{1}, integer2{2}
        ));
    }
}
