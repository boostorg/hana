/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/group/operators.hpp>

#include <test/laws/group.hpp>
#include <test/numeric/comparable.hpp>
#include <test/numeric/group.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <> struct enabled_operators<test::Numeric> : Group { };
}}

int main() {
    constexpr int x = 2, y = 3;

    // minus
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            minus(test::numeric(x), test::numeric(y)),
            test::numeric(x - y)
        ));
    }

    // negate
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            negate(test::numeric(x)),
            test::numeric(-x)
        ));
    }

    // operators
    {
        using namespace boost::hana::operators;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            test::numeric(x) - test::numeric(y),
            minus(test::numeric(x), test::numeric(y))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            -test::numeric(x),
            negate(test::numeric(x))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Group_laws<test::Numeric>(
            test::numeric(0), test::numeric(1), test::numeric(2), test::numeric(3)
        ));
    }
}
