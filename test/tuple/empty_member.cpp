/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;

//
// This test makes sure that `hana::tuple` has a simple enough representation.
// In particular, we make sure that the empty base-class optimization (EBO)
// is able to fire, when supported.
//

template <typename T>
struct holder { T value; };

template <typename ...T>
struct inherit : T... { };

struct A { };
struct B { };

static_assert((sizeof(hana::tuple<int, A>) == sizeof(inherit<holder<int>, A>)), "");
static_assert((sizeof(hana::tuple<A, int>) == sizeof(inherit<A, holder<int>>)), "");
static_assert((sizeof(hana::tuple<A, int, B>) == sizeof(inherit<A, holder<int>, B>)), "");
static_assert((sizeof(hana::tuple<A, B, int>) == sizeof(inherit<A, B, holder<int>>)), "");
static_assert((sizeof(hana::tuple<int, A, B>) == sizeof(inherit<holder<int>, A, B>)), "");

int main() { }
