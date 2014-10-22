/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/orderable.hpp>
#include <ratio>
using namespace boost::hana;


int main() {
    // less
    {
        BOOST_HANA_CONSTANT_ASSERT(less(
            std::ratio<1>{},
            std::ratio<3>{}
        ));

        BOOST_HANA_CONSTANT_ASSERT(less(
            std::ratio<4, 10>{},
            std::ratio<3, 5>{}
        ));

        BOOST_HANA_CONSTANT_ASSERT(not_(less(
            std::ratio<3, 5>{},
            std::ratio<4, 10>{}
        )));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Orderable_laws(
            std::ratio<0>{},
            std::ratio<1, 2>{},
            std::ratio<1, 3>{},
            std::ratio<3, 5>{},
            std::ratio<2, 1>{}
        ));
    }
}
