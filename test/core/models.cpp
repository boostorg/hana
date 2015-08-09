/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/models.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/concept/iterable.hpp>
namespace hana = boost::hana;


// Make sure senseless crap works as expected.
static_assert(!hana::models<hana::Iterable, void>, "");
static_assert(!hana::models<void, hana::IntegralConstant<int>>, "");
static_assert(!hana::models<void, void>, "");

int main() { }
