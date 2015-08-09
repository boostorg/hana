/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
namespace hana = boost::hana;


// empty ranges are equal
BOOST_HANA_CONSTANT_CHECK(hana::make_range(hana::int_<6>, hana::int_<6>) == hana::make_range(hana::int_<0>, hana::int_<0>));

// otherwise, ranges are equal if and only if they span the same interval
BOOST_HANA_CONSTANT_CHECK(hana::make_range(hana::int_<2>, hana::int_<5>) == hana::make_range(hana::int_<2>, hana::int_<5>));
BOOST_HANA_CONSTANT_CHECK(hana::make_range(hana::int_<0>, hana::int_<3>) != hana::make_range(hana::int_<-1>, hana::int_<3>));

int main() { }
