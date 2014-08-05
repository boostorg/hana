/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;


//! [main]
BOOST_HANA_CONSTANT_ASSERT(head(range_c<int, 0, 5>) == int_<0>);
BOOST_HANA_CONSTANT_ASSERT(last(range_c<unsigned long, 0, 5>) == ulong<4>);
BOOST_HANA_CONSTANT_ASSERT(tail(range_c<int, 0, 5>) == range(int_<1>, int_<5>));
//! [main]

int main() { }
