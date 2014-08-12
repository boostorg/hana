/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/laws/comparable.hpp>
using namespace boost::hana;


int main() {
    auto x = test::x<0>;
    auto y = test::x<1>;

    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(nothing, nothing));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(nothing, just(x))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(just(x), nothing)));
        BOOST_HANA_CONSTANT_ASSERT(equal(just(x), just(x)));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(just(x), just(y))));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            nothing, just(x), just(y)
        ));
    }
}
