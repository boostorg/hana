// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/core/to.hpp>
namespace hana = boost::hana;


// This test makes sure that fundamental types are properly embedded in
// each other, when sensible.

static_assert(hana::is_embedded<double, long double>{}, "");
static_assert(hana::is_embedded<float, long double>{}, "");
static_assert(hana::is_embedded<float, double>{}, "");

static_assert(hana::is_embedded<long,        long long>{}, "");
static_assert(hana::is_embedded<int,         long long>{}, "");
static_assert(hana::is_embedded<short,       long long>{}, "");
static_assert(hana::is_embedded<signed char, long long>{}, "");
static_assert(hana::is_embedded<int,         long>{}, "");
static_assert(hana::is_embedded<short,       long>{}, "");
static_assert(hana::is_embedded<signed char, long>{}, "");
static_assert(hana::is_embedded<short,       int>{}, "");
static_assert(hana::is_embedded<signed char, int>{}, "");

static_assert(hana::is_embedded<unsigned long,  unsigned long long>{}, "");
static_assert(hana::is_embedded<unsigned int,   unsigned long long>{}, "");
static_assert(hana::is_embedded<unsigned short, unsigned long long>{}, "");
static_assert(hana::is_embedded<unsigned char,  unsigned long long>{}, "");
static_assert(hana::is_embedded<unsigned int,   unsigned long>{}, "");
static_assert(hana::is_embedded<unsigned short, unsigned long>{}, "");
static_assert(hana::is_embedded<unsigned char,  unsigned long>{}, "");
static_assert(hana::is_embedded<unsigned short, unsigned int>{}, "");
static_assert(hana::is_embedded<unsigned char,  unsigned int>{}, "");

int main() { }
