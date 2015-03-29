/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional/capture.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
using namespace boost::hana;


int main() {
//! [main]
BOOST_HANA_CONSTEXPR_LAMBDA auto sum = [](auto x, auto y, auto z) {
    return x + y + z;
};

BOOST_HANA_CONSTEXPR_CHECK(capture(1, 2, 3)(sum)() == 6);
BOOST_HANA_CONSTEXPR_CHECK(capture(1, 2)(sum)(3) == 6);
//! [main]
}
