/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find_if.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


constexpr auto x = hana::find_if(hana::make_tuple(hana::int_c<0>), hana::id);
BOOST_HANA_CONSTANT_CHECK(hana::equal(x, hana::nothing));

constexpr auto y = hana::find_if(hana::make_tuple(hana::int_c<1>), hana::id);
BOOST_HANA_CONSTANT_CHECK(hana::equal(y, hana::just(hana::int_c<1>)));

constexpr auto z = hana::find_if(hana::make_tuple(hana::int_c<3>), hana::id);
BOOST_HANA_CONSTANT_CHECK(hana::equal(z, hana::just(hana::int_c<3>)));

int main() { }
