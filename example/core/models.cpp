/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


static_assert(hana::models<hana::Iterable, hana::Tuple>, "");
static_assert(!hana::models<hana::Iterable, hana::IntegralConstant<int>>, "");

int main() { }
