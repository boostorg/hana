/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral_domain/operators.hpp>

#include <test/laws/integral_domain.hpp>
#include <test/numeric/comparable.hpp>
#include <test/numeric/integral_domain.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <> struct enabled_operators<test::Numeric> : IntegralDomain { };
}}

int main() {
    constexpr int x = 6, y = 3, z = 4;

    // quot
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            quot(test::numeric(x), test::numeric(y)),
            test::numeric(x / y)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            quot(test::numeric(x), test::numeric(z)),
             test::numeric(x/ z)
        ));
    }

    // mod
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mod(test::numeric(x), test::numeric(y)),
            test::numeric(x % y)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mod(test::numeric(x), test::numeric(z)),
            test::numeric(x % z)
        ));
    }

    // operators
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            test::numeric(x) % test::numeric(y),
            mod(test::numeric(x), test::numeric(y))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            test::numeric(x) / test::numeric(y),
            quot(test::numeric(x), test::numeric(y))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(IntegralDomain_laws<test::Numeric>(
            test::numeric(0), test::numeric(1), test::numeric(2), test::numeric(3)
        ));
    }
}
