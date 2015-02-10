/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
using namespace boost::hana;


int main() {

{

//! [zero]
BOOST_HANA_CONSTANT_CHECK(zero<IntegralConstant<int>>() == int_<0>);
BOOST_HANA_CONSTEXPR_CHECK(zero<long>() == 0l);
//! [zero]

}{

//! [plus]
BOOST_HANA_CONSTANT_CHECK(plus(int_<3>, int_<5>) == int_<8>);
BOOST_HANA_CONSTEXPR_CHECK(plus(1, 2) == 3);
BOOST_HANA_CONSTEXPR_CHECK(plus(1.5f, 2.4) == 3.9);
//! [plus]

}

}
