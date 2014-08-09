/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/monoid/detail/laws.hpp>
using namespace boost::hana;


int main() {
    // zero
    {
        BOOST_HANA_CONSTANT_ASSERT(zero<Integral> == int_<0>);
    }

    // plus
    {
        BOOST_HANA_CONSTANT_ASSERT(plus(int_<3>, int_<4>) == int_<3 + 4>);
        BOOST_HANA_CONSTEXPR_ASSERT(plus(int_<3>, 4) == 3 + 4);
        BOOST_HANA_CONSTEXPR_ASSERT(plus(3, int_<4>) == 3 + 4);
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Monoid::laws::check(list(
            int_<1>, short_<2>, long_<3>, ullong<4>, 5, 6ull
        )));
    }
}
