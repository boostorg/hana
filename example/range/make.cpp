/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
namespace hana = boost::hana;


constexpr auto irange = hana::make<hana::Range>(hana::int_<0>, hana::int_<10>); // [0, 10) int
constexpr auto lrange = hana::make<hana::Range>(hana::int_<0>, hana::long_<10>); // [0, 10) long
BOOST_HANA_CONSTANT_CHECK(lrange == hana::make<hana::Range>(hana::long_<0>, hana::long_<10>));

int main() { }
