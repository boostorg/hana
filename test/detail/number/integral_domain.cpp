/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_domain/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/detail/number/integral_domain.hpp>
#include <boost/hana/integral_domain/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto n = detail::number<Mcd>;
    // quot
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            quot(n(6), n(3)), n(6 / 3)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            quot(n(6), n(4)), n(6 / 4)
        ));
    }

    // mod
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mod(n(6), n(3)), n(6 % 3)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            mod(n(6), n(4)), n(6 % 4)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(IntegralDomain::laws::check(
            list(n(0), n(1), n(2), n(3))
        ));
    }

    // operators
    {
        constexpr auto n = detail::number<Mcd, operators<IntegralDomain>>;
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            n(6) % n(3),
            mod(n(6), n(3))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            n(6) / n(3),
            quot(n(6), n(3))
        ));
    }
}

int main() {
    test<IntegralDomain::mcd>();
}
