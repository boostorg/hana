/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/comparable.hpp>
using namespace boost::hana;


int main() {
    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(true_, true_));
        BOOST_HANA_CONSTANT_ASSERT(equal(false_, false_));

        BOOST_HANA_CONSTANT_ASSERT(not_(equal(false_, true_)));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(true_, false_)));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(true_, false_));
    }
}
