/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
namespace hana = boost::hana;


template <typename ...> struct f;
struct x;
struct y;

BOOST_HANA_CONSTANT_CHECK(hana::template_<f>() == hana::type<f<>>);
BOOST_HANA_CONSTANT_CHECK(hana::template_<f>(hana::type<x>) == hana::type<f<x>>);
BOOST_HANA_CONSTANT_CHECK(hana::template_<f>(hana::type<x>, hana::type<y>) == hana::type<f<x, y>>);

// calling `hana::template_` on non-Types
BOOST_HANA_CONSTANT_CHECK(hana::template_<f>(1) == hana::type<f<int>>);

static_assert(std::is_same<
    decltype(hana::template_<f>)::apply<x, y>::type,
    f<x, y>
>::value, "");

int main() { }
