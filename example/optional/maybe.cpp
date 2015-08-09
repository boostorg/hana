/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/optional.hpp>
#include <boost/hana/succ.hpp>
namespace hana = boost::hana;


static_assert(hana::maybe('x', hana::succ, hana::just(1)) == 2, "");
static_assert(hana::maybe('x', hana::succ, hana::nothing) == 'x', "");

int main() { }
