/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ring/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/ring/detail/minimal.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto r = minimal_ring<mcd>;
    BOOST_HANA_CONSTEXPR_ASSERT(equal(mult(r(2), r(3)), r(2 * 3)));
}

int main() {
    test<Ring::mcd>();
}
