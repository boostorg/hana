/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


static_assert(hana::is_a<hana::Tuple, decltype(hana::make_tuple(1, '2', 3.3))>, "");
static_assert(!hana::is_a<hana::Tuple, void>, "");
static_assert(hana::is_an<hana::IntegralConstant<int>>(hana::int_c<10>), "");

int main() { }
