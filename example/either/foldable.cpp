/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/unpack.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(hana::unpack(hana::left('x'), hana::make_tuple) == hana::make_tuple());
static_assert(hana::unpack(hana::right('x'), hana::make_tuple) == hana::make_tuple('x'), "");

int main() { }
