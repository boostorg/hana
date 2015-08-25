/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/back.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tail.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(hana::front(hana::range_c<int, 0, 5>) == hana::int_<0>);
BOOST_HANA_CONSTANT_CHECK(hana::back(hana::range_c<unsigned long, 0, 5>) == hana::ulong<4>);
BOOST_HANA_CONSTANT_CHECK(hana::tail(hana::range_c<int, 0, 5>) == hana::make_range(hana::int_<1>, hana::int_<5>));

int main() { }
