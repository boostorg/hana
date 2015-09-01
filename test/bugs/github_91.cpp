/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/group.hpp>
#include <boost/hana/concept/integral_domain.hpp>
#include <boost/hana/concept/monoid.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/concept/ring.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/integral_constant.hpp>
namespace hana = boost::hana;


using T = decltype(hana::int_c<1>);

static_assert(hana::models<hana::Comparable, T>(), "");
static_assert(hana::models<hana::Orderable, T>(), "");
static_assert(hana::models<hana::Monoid, T>(), "");
static_assert(hana::models<hana::Group, T>(), "");
static_assert(hana::models<hana::Ring, T>(), "");
static_assert(hana::models<hana::IntegralDomain, T>(), "");

int main() { }
