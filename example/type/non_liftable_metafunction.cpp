/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/type_traits.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/value.hpp>

#include <type_traits>
namespace hana = boost::hana;


auto extent = [](auto t, auto n) {
    return std::extent<typename decltype(t)::type, hana::value(n)>{};
};
BOOST_HANA_CONSTANT_CHECK(extent(hana::type<char>, hana::int_<1>) == hana::size_t<0>);
BOOST_HANA_CONSTANT_CHECK(extent(hana::type<char[1][2]>, hana::int_<1>) == hana::size_t<2>);

int main() { }
