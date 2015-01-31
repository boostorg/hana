/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {

{

//! [comparable]
// empty ranges are equal
BOOST_HANA_CONSTANT_CHECK(range(int_<6>, int_<6>) == range(int_<0>, int_<0>));

// otherwise, ranges are equal if and only if they span the same interval
BOOST_HANA_CONSTANT_CHECK(range(int_<2>, int_<5>) == range(int_<2>, int_<5>));
BOOST_HANA_CONSTANT_CHECK(range(int_<0>, int_<3>) != range(int_<-1>, int_<3>));
//! [comparable]

}{

//! [foldable]
BOOST_HANA_CONSTANT_CHECK(foldl(range(int_<0>, int_<4>), int_<0>, plus) == int_<6>);
BOOST_HANA_CONSTANT_CHECK(unpack(range(int_<-2>, int_<2>), tuple) == tuple(int_<-2>, int_<-1>, int_<0>, int_<1>));
//! [foldable]

}{

//! [iterable]
constexpr auto r = range(int_<0>, int_<1000>);
BOOST_HANA_CONSTANT_CHECK(head(r) == int_<0>);
BOOST_HANA_CONSTANT_CHECK(last(r) == int_<999>);
BOOST_HANA_CONSTANT_CHECK(!is_empty(r));
BOOST_HANA_CONSTANT_CHECK(is_empty(range(int_<3>, int_<3>)));
//! [iterable]

}{

//! [range_c]
BOOST_HANA_CONSTANT_CHECK(head(range_c<int, 0, 5>) == int_<0>);
BOOST_HANA_CONSTANT_CHECK(last(range_c<unsigned long, 0, 5>) == ulong<4>);
BOOST_HANA_CONSTANT_CHECK(tail(range_c<int, 0, 5>) == range(int_<1>, int_<5>));
//! [range_c]

}{

//! [range]
using namespace literals;

BOOST_HANA_CONSTANT_CHECK(head(range(0_c, 5_c)) == 0_c);
BOOST_HANA_CONSTANT_CHECK(last(range(0_c, 5_c)) == 4_c);
BOOST_HANA_CONSTANT_CHECK(tail(range(0_c, 5_c)) == range(1_c, 5_c));
//! [range]

}

}
