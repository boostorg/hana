/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral_domain/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    // quot
    {
        BOOST_HANA_CONSTANT_ASSERT(quot(int_<6>, int_<4>) == int_<6 / 4>);
        BOOST_HANA_CONSTEXPR_ASSERT(quot(int_<6>, 4) == 6 / 4);
        BOOST_HANA_CONSTEXPR_ASSERT(quot(6, int_<4>) == 6 / 4);
    }

    // mod
    {
        BOOST_HANA_CONSTANT_ASSERT(mod(int_<6>, int_<4>) == int_<6 % 4>);
        BOOST_HANA_CONSTEXPR_ASSERT(mod(int_<6>, 4) == 6 % 4);
        BOOST_HANA_CONSTEXPR_ASSERT(mod(6, int_<4>) == 6 % 4);
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(IntegralDomain::laws::check(list(
            int_<1>, short_<2>, long_<3>, ullong<4>
        )));
    }
}
