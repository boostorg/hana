/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/zero.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(hana::zero<hana::IntegralConstant<int>>() == hana::int_c<0>);
static_assert(hana::zero<long>() == 0l, "");

int main() { }
