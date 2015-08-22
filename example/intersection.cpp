/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/intersection.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


constexpr auto xs = hana::make_set(hana::int_<1>, hana::type<void>, hana::int_<2>);
constexpr auto ys = hana::make_set(hana::int_<2>, hana::type<int>, hana::int_<3>);

BOOST_HANA_CONSTANT_CHECK(hana::intersection(xs, ys) == hana::make_set(hana::int_<2>));

int main() { }
