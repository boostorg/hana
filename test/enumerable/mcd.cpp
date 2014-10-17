/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>

#include <test/numeric/comparable.hpp>
#include <test/numeric/enumerable.hpp>
using namespace boost::hana;


int main() {
    // succ
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            succ(test::numeric(0)),
            test::numeric(1)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            succ(test::numeric(1)),
            test::numeric(2)
        ));
    }

    // pred
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            pred(test::numeric(1)),
            test::numeric(0)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            pred(test::numeric(2)),
            test::numeric(1)
        ));
    }
}
