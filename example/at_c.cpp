/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/at.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


constexpr auto xs = hana::make_tuple(0, '1', 2.0);

static_assert(hana::at_c<0>(xs) == 0, "");
static_assert(hana::at_c<1>(xs) == '1', "");
static_assert(hana::at_c<2>(xs) == 2.0, "");

int main() { }
