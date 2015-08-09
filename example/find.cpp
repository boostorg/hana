/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(
    hana::find(hana::make_tuple(hana::int_<1>, hana::type<int>, '3'), hana::type<int>) == hana::just(hana::type<int>)
);
BOOST_HANA_CONSTANT_CHECK(
    hana::find(hana::make_tuple(hana::int_<1>, hana::type<int>, '3'), hana::type<void>) == hana::nothing
);

constexpr auto m = hana::make_map(
    hana::make_pair(hana::int_<2>, 2),
    hana::make_pair(hana::type<float>, 3.3),
    hana::make_pair(hana::type<char>, hana::type<int>)
);
static_assert(hana::find(m, hana::type<float>) == hana::just(3.3), "");

int main() { }
