/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/find_if.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
namespace hana = boost::hana;


static_assert(
    hana::find_if(hana::make_tuple(1.0, 2, '3'), hana::trait<std::is_integral>) == hana::just(2)
, "");

BOOST_HANA_CONSTANT_CHECK(
    hana::find_if(hana::make_tuple(1.0, 2, '3'), hana::trait<std::is_class>) == hana::nothing
);

constexpr auto types = hana::tuple_t<char, int, unsigned, long, unsigned long>;
BOOST_HANA_CONSTANT_CHECK(
    hana::find_if(types, hana::equal.to(hana::type_c<unsigned>)) == hana::just(hana::type_c<unsigned>)
);
BOOST_HANA_CONSTANT_CHECK(
    hana::find_if(types, hana::equal.to(hana::type_c<void>)) == hana::nothing
);

int main() { }
