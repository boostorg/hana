/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/array.hpp>
using namespace boost::hana;
using sandbox::array;


constexpr array<int, 4> a = {{0, 1, 2, 3}};
constexpr array<int, 6> b = {{5, 4, 3, 2, 1, 0}};

static_assert(a[0] == 0, "");
static_assert(a[1] == 1, "");
static_assert(a[2] == 2, "");
static_assert(a[3] == 3, "");

static_assert(a.at(0) == 0, "");
static_assert(a.at(1) == 1, "");
static_assert(a.at(2) == 2, "");

static_assert(a != b, "");
static_assert(a < b, "");


int main() { }
