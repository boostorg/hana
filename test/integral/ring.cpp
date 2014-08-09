/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/ring/detail/laws.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTANT_ASSERT(one<Integral> == int_<1>);
    BOOST_HANA_CONSTANT_ASSERT(mult(int_<3>, int_<4>) == int_<3 * 4>);
    BOOST_HANA_CONSTEXPR_ASSERT(mult(int_<3>, 4) == 3 * 4);
    BOOST_HANA_CONSTEXPR_ASSERT(mult(3, int_<4>) == 3 * 4);

    BOOST_HANA_CONSTEXPR_ASSERT(Ring::laws::check(list(
        int_<1>, short_<2>, long_<3>, ullong<4>, 5, 6ull
    )));
}
