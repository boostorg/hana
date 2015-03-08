/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/models.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


//! [models]
static_assert(models<Iterable, Tuple>{}, "");
static_assert(!models<Iterable, IntegralConstant<int>>{}, "");
//! [models]

// Make sure senseless crap works as expected. This should be part
// of the unit tests.
static_assert(!models<Iterable, void>{}, "");
static_assert(!models<void, IntegralConstant<int>>{}, "");
static_assert(!models<void, void>{}, "");

int main() { }
