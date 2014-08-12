/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/monoid/operators.hpp>

#include <test/laws/monoid.hpp>
#include <test/numeric/comparable.hpp>
#include <test/numeric/monoid.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <> struct enabled_operators<test::Numeric> : Monoid { };
}}

int main() {
    constexpr int x = 2, y = 3;

    // zero
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            zero<test::Numeric>, test::numeric(0)
        ));
    }

    // plus
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            plus(test::numeric(x), test::numeric(y)),
            test::numeric(x + y)
        ));
    }

    // operators
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            test::numeric(x) + test::numeric(y),
            plus(test::numeric(x), test::numeric(y))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Monoid_laws<test::Numeric>(
            test::numeric(0), test::numeric(1), test::numeric(2), test::numeric(3)
        ));
    }
}
