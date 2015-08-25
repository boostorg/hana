/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/equal.hpp>
#include <boost/hana/take.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


static_assert(hana::take_c<2>(hana::make_tuple(1, '2', 3.3)) == hana::make_tuple(1, '2'), "");

int main() { }
