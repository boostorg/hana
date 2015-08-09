/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ap.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/succ.hpp>
namespace hana = boost::hana;


static_assert(hana::ap(hana::just(hana::succ), hana::just('x')) == hana::just('y'), "");
BOOST_HANA_CONSTANT_CHECK(hana::ap(hana::nothing, hana::just('x')) == hana::nothing);
BOOST_HANA_CONSTANT_CHECK(hana::ap(hana::just(hana::succ), hana::nothing) == hana::nothing);
BOOST_HANA_CONSTANT_CHECK(hana::ap(hana::nothing, hana::nothing) == hana::nothing);

int main() { }
