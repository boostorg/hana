/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    //! [mod]
    BOOST_HANA_CONSTANT_ASSERT(mod(int_<6>, int_<4>) == int_<2>);
    BOOST_HANA_CONSTEXPR_ASSERT(mod(6, 4) == 2);
    //! [mod]

    //! [quot]
    BOOST_HANA_CONSTANT_ASSERT(quot(int_<6>, int_<3>) == int_<2>);
    BOOST_HANA_CONSTANT_ASSERT(quot(int_<6>, int_<4>) == int_<1>);

    BOOST_HANA_CONSTEXPR_ASSERT(quot(6, 3) == 2);
    BOOST_HANA_CONSTEXPR_ASSERT(quot(6, 4) == 1);
    //! [quot]
}
