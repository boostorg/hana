/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral/comparable.hpp>
#include <boost/hana/integral/integral_constant.hpp>

#include <boost/hana/detail/assert.hpp>
using namespace boost::hana;


using T = int;
constexpr T x = 3;

int main() {
    BOOST_HANA_CONSTANT_ASSERT(equal(
        integral_constant<Integral, T, x>,
        integral<T, x>
    ));
}
