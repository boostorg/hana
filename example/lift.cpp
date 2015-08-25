/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/equal.hpp>
#include <boost/hana/lift.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


static_assert(hana::lift<hana::Tuple>('x') == hana::make_tuple('x'), "");
static_assert(hana::lift<hana::Optional>('x') == hana::just('x'), "");

int main() { }
