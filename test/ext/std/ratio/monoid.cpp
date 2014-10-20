/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio/monoid.hpp>
#include <boost/hana/ext/std/ratio/comparable.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/monoid.hpp>
#include <ratio>
using namespace boost::hana;


int main() {
    // plus
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            plus(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
            std::ratio<3*10 + 5*4, 4*10>{}
        ));
    }

    // zero
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zero<StdRatio>,
            std::ratio<0, 1>{}
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            zero<StdRatio>,
            std::ratio<0, 2>{}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Monoid_laws<StdRatio>(
            std::ratio<0>{},
            std::ratio<1, 2>{},
            std::ratio<1, 3>{},
            std::ratio<3, 5>{},
            std::ratio<2, 1>{}
        ));
    }
}
