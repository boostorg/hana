/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/none.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


static_assert(hana::none(hana::make_tuple(false, hana::false_, hana::false_)), "");
static_assert(!hana::none(hana::make_tuple(false, hana::false_, true)), "");
BOOST_HANA_CONSTANT_CHECK(!hana::none(hana::make_tuple(false, hana::false_, hana::true_)));

int main() { }
