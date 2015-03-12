/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


//! [is_a]
static_assert(is_a<Tuple, decltype(make_tuple(1, '2', 3.3))>, "");
static_assert(!is_a<Tuple, void>, "");
static_assert(is_an<IntegralConstant<int>>(int_<10>), "");
//! [is_a]

int main() { }
