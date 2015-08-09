/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/back.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tail.hpp>
namespace hana = boost::hana;


constexpr auto r = hana::make_range(hana::int_<0>, hana::int_<1000>);
BOOST_HANA_CONSTANT_CHECK(hana::front(r) == hana::int_<0>);
BOOST_HANA_CONSTANT_CHECK(hana::back(r) == hana::int_<999>);
BOOST_HANA_CONSTANT_CHECK(hana::tail(r) == hana::make_range(hana::int_<1>, hana::int_<1000>));
BOOST_HANA_CONSTANT_CHECK(!hana::is_empty(r));
BOOST_HANA_CONSTANT_CHECK(hana::is_empty(hana::make_range(hana::int_<3>, hana::int_<3>)));

int main() { }
