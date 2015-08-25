/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


struct X { };
BOOST_HANA_CONSTANT_CHECK(hana::decltype_(X{}) == hana::type<X>);
BOOST_HANA_CONSTANT_CHECK(hana::decltype_(hana::type<X>) == hana::type<X>);

BOOST_HANA_CONSTANT_CHECK(hana::decltype_(1) == hana::type<int>);

static int const& i = 1;
BOOST_HANA_CONSTANT_CHECK(hana::decltype_(i) == hana::type<int const>);

int main() { }
