/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>

#include <ratio>
using namespace boost::hana;


int main() {
    auto ratios = make<Tuple>(
          std::ratio<0>{}
        , std::ratio<1, 3>{}
        , std::ratio<1, 2>{}
        , std::ratio<2, 6>{}
        , std::ratio<3, 1>{}
        , std::ratio<7, 8>{}
        , std::ratio<3, 5>{}
        , std::ratio<2, 1>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                std::ratio<0>{},
                std::ratio<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                std::ratio<3, 5>{},
                std::ratio<6, 10>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                std::ratio<4, 5>{},
                std::ratio<6, 10>{}
            )));
        }

        // laws
        test::TestComparable<ext::std::Ratio>{ratios};
    }
}
