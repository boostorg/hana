/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/ring.hpp>
#include <ratio>
using namespace boost::hana;


int main() {
    // mult
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            mult(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
            std::ratio<3*5, 4*10>{}
        ));
    }

    // one
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            one<ext::std::Ratio>,
            std::ratio<1, 1>{}
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            one<ext::std::Ratio>,
            std::ratio<2, 2>{}
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Ring_laws<ext::std::Ratio>(
            std::ratio<0>{},
            std::ratio<1, 2>{},
            std::ratio<1, 3>{},
            std::ratio<3, 5>{},
            std::ratio<2, 1>{}
        ));
    }
}
