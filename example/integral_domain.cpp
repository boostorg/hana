/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
using namespace boost::hana;


int main() {

{

//! [rem]
BOOST_HANA_CONSTANT_CHECK(rem(int_<6>, int_<4>) == int_<2>);
BOOST_HANA_CONSTANT_CHECK(rem(int_<-6>, int_<4>) == int_<-2>);
BOOST_HANA_CONSTEXPR_CHECK(rem(6, 4) == 2);
//! [rem]

}{

//! [quot]
BOOST_HANA_CONSTANT_CHECK(quot(int_<6>, int_<3>) == int_<2>);
BOOST_HANA_CONSTANT_CHECK(quot(int_<6>, int_<4>) == int_<1>);

BOOST_HANA_CONSTEXPR_CHECK(quot(6, 3) == 2);
BOOST_HANA_CONSTEXPR_CHECK(quot(6, 4) == 1);
//! [quot]

}

}
