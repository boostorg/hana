/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/ring.hpp>

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
    // Ring
    //////////////////////////////////////////////////////////////////////////
    {
        // mult
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                mult(std::ratio<3, 4>{}, std::ratio<5, 10>{}),
                std::ratio<3*5, 4*10>{}
            ));
        }

        // one
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                one<ext::std::Ratio>(),
                std::ratio<1, 1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                one<ext::std::Ratio>(),
                std::ratio<2, 2>{}
            ));
        }

        // laws
        test::TestRing<ext::std::Ratio>{ratios};
    }
}
