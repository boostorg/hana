/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/either.hpp>
namespace hana = boost::hana;


// left is always less than right, regardless of what's in it
BOOST_HANA_CONSTANT_CHECK(hana::left(2000) < hana::right(2));

// when comparing two lefts or two rights, we compare the contents
static_assert(hana::left(2) < hana::left(2000), "");
static_assert(hana::right(2) < hana::right(2000), "");

int main() { }
