/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/either.hpp>
namespace hana = boost::hana;


static_assert(hana::left('x') == hana::left('x'), "");
BOOST_HANA_CONSTANT_CHECK(hana::right('x') != hana::left('x'));
static_assert(hana::right('x') == hana::right('x'), "");
static_assert(hana::right('x') != hana::right('y'), "");

int main() { }
