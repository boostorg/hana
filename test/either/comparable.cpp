/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either/comparable.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/laws/comparable.hpp>
using namespace boost::hana;


int main() {
    auto x = test::x<0>;
    auto y = test::x<1>;
    constexpr struct { } undefined{};

    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(left(x), left(x)));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(left(x), left(y))));

        BOOST_HANA_CONSTANT_ASSERT(not_(equal(left(undefined), right(undefined))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(right(undefined), left(undefined))));

        BOOST_HANA_CONSTANT_ASSERT(equal(right(x), right(x)));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(right(x), right(y))));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            left(x), left(y),
            right(x), right(y)
        ));
    }
}
