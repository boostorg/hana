/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
namespace hana = boost::hana;


struct f { template <typename ...> struct apply { struct type; }; };
struct x;
struct y;

BOOST_HANA_CONSTANT_CHECK(hana::metafunction_class<f>() == hana::type<f::apply<>::type>);
BOOST_HANA_CONSTANT_CHECK(hana::metafunction_class<f>(hana::type<x>) == hana::type<f::apply<x>::type>);
BOOST_HANA_CONSTANT_CHECK(hana::metafunction_class<f>(hana::type<x>, hana::type<y>) == hana::type<f::apply<x, y>::type>);

// calling `hana::metafunction_class` on non-Types
BOOST_HANA_CONSTANT_CHECK(hana::metafunction_class<f>(1) == hana::type<f::apply<int>::type>);

static_assert(std::is_same<
    decltype(hana::metafunction_class<f>)::apply<x, y>::type,
    f::apply<x, y>::type
>::value, "");

int main() { }
