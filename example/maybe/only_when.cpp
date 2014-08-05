/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


//! [main]
BOOST_HANA_CONSTEXPR_LAMBDA auto even = [](auto x) {
    return x % int_<2> == int_<0>;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
    return x / int_<2>;
};

BOOST_HANA_CONSTANT_ASSERT(only_when(even, half, int_<4>) == just(int_<2>));
BOOST_HANA_CONSTANT_ASSERT(only_when(even, half, int_<3>) == nothing);
//! [main]

int main() { (void)even; (void)half; }
