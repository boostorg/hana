/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/ratio/convert.hpp>
#include <boost/hana/ext/std/ratio/comparable.hpp>

#include <boost/hana/detail/assert.hpp>

#include <ratio>
#include <test/cnumeric/integral_constant.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTANT_ASSERT(equal(
        to<StdRatio>(test::cnumeric<int, 0>),
        std::ratio<0>{}
    ));

    BOOST_HANA_CONSTANT_ASSERT(equal(
        to<StdRatio>(test::cnumeric<int, 1>),
        std::ratio<1>{}
    ));

    BOOST_HANA_CONSTANT_ASSERT(equal(
        to<StdRatio>(test::cnumeric<int, 3>),
        std::ratio<3>{}
    ));
}
