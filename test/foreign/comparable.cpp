/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foreign.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/detail/assert.hpp>

#define BOOST_HANA_TEST_COMPARABLE
#include <test/builtin_integer.hpp>
#include <test/laws/comparable.hpp>
using namespace boost::hana;


int main() {
    // equal
    {
        // same type
        BOOST_HANA_CONSTEXPR_ASSERT(equal(integer{0}, integer{0}));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(integer{0}, integer{1})));
        BOOST_HANA_CONSTANT_ASSERT(are<Comparable>(integer{0}, integer{0}));

        // mixed types
        BOOST_HANA_CONSTEXPR_ASSERT(equal(integer{0}, integer2{0}));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(integer{0}, integer2{1})));
        BOOST_HANA_CONSTANT_ASSERT(are<Comparable>(integer{0}, integer2{0}));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Comparable_laws(
            integer{0}, integer{1}, integer{2},
            integer2{0}, integer2{1}, integer2{2}
        ));
    }
}
