/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/optional.hpp>
namespace hana = boost::hana;


static_assert(hana::from_maybe('x', hana::just(1)) == 1, "");
static_assert(hana::from_maybe('x', hana::nothing) == 'x', "");

int main() { }
