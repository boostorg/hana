/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/either.hpp>
namespace hana = boost::hana;


constexpr auto left_value = hana::left('x');
static_assert(hana::is_a<hana::Either>(left_value), "");

int main() { }
