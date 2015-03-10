/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio.hpp>

#include <boost/hana/assert.hpp>

#include <test/cnumeric.hpp>

#include <ratio>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    {
        // Constant -> Ratio
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<ext::std::Ratio>(test::cnumeric<int, 0>),
                std::ratio<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<ext::std::Ratio>(test::cnumeric<int, 1>),
                std::ratio<1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<ext::std::Ratio>(test::cnumeric<int, 3>),
                std::ratio<3>{}
            ));
        }
    }
}
