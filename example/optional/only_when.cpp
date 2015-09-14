/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/div.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/mod.hpp>
#include <boost/hana/optional.hpp>
namespace hana = boost::hana;


auto even = [](auto x) {
    return x % hana::int_c<2> == hana::int_c<0>;
};

auto half = [](auto x) {
    return x / hana::int_c<2>;
};

BOOST_HANA_CONSTANT_CHECK(hana::only_when(even, half, hana::int_c<4>) == hana::just(hana::int_c<2>));
BOOST_HANA_CONSTANT_CHECK(hana::only_when(even, half, hana::int_c<3>) == hana::nothing);

int main() { }
