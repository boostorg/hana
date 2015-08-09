/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/pred.hpp>
#include <boost/hana/succ.hpp>
namespace hana = boost::hana;


static_assert(hana::either(hana::succ, hana::pred, hana::left(1)) == 2, "");
static_assert(hana::either(hana::succ, hana::pred, hana::right(1)) == 0, "");

int main() { }
