/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/succ.hpp>
#include <boost/hana/transform.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(hana::transform(hana::nothing, hana::succ) == hana::nothing);
static_assert(hana::transform(hana::just(1), hana::succ) == hana::just(2), "");

int main() { }
