// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/concept/euclidean_ring.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not.hpp>

#include <support/cnumeric.hpp>
namespace hana = boost::hana;


static_assert(!hana::EuclideanRing<unsigned short>{}, "");
static_assert(!hana::EuclideanRing<unsigned int>{}, "");
static_assert(!hana::EuclideanRing<unsigned long>{}, "");
static_assert(!hana::EuclideanRing<unsigned long long>{}, "");

template <typename T, T v>
using minimal_constant = ::cnumeric_t<T, v>;
static_assert(!hana::EuclideanRing<minimal_constant<unsigned short, 10>>{}, "");
static_assert(!hana::EuclideanRing<minimal_constant<unsigned int, 10>>{}, "");
static_assert(!hana::EuclideanRing<minimal_constant<unsigned long, 10>>{}, "");
static_assert(!hana::EuclideanRing<minimal_constant<unsigned long long, 10>>{}, "");

static_assert(!hana::EuclideanRing<hana::integral_constant<unsigned int, 10>>{}, "");

int main() { }
