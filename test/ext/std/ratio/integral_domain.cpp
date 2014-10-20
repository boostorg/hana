/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio/integral_domain.hpp>
#include <boost/hana/ext/std/ratio/comparable.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/integral_domain.hpp>
#include <ratio>
using namespace boost::hana;


int main() {
    // quot
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            quot(std::ratio<6>{}, std::ratio<4>{}),
            std::ratio<6, 4>{}
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            quot(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
            std::ratio<3*10, 4*5>{}
        ));
    }

    // mod
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            mod(std::ratio<6>{}, std::ratio<4>{}),
            std::ratio<0>{}
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            mod(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
            std::ratio<0>{}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(IntegralDomain_laws<StdRatio>(
            std::ratio<0>{},
            std::ratio<1, 2>{},
            std::ratio<1, 3>{},
            std::ratio<3, 5>{},
            std::ratio<2, 1>{}
        ));
    }
}
