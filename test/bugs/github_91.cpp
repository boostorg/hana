/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/models.hpp>
#include <boost/hana/integral_constant.hpp>
using namespace boost::hana;


using T = decltype(int_<1>);

static_assert(models<Comparable, T>(), "");
static_assert(models<Orderable, T>(), "");
static_assert(models<Monoid, T>(), "");
static_assert(models<Group, T>(), "");
static_assert(models<Ring, T>(), "");
static_assert(models<IntegralDomain, T>(), "");

int main() { }
