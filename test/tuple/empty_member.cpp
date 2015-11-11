/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


//
// We disable this test when the compiler does not support the empty base
// class optimization (EBO). Compilers normally support it, but Clang for
// Windows emulates the broken behaviour of MSVC, and does not support the
// EBO for more than one base class. See https://goo.gl/5FE3R6 for more info.
//

template <typename T>
struct holder { T value; };

template <typename ...T>
struct inherit : T... { };

struct A { };
struct B { };
static constexpr bool EBO_is_supported = sizeof(inherit<A, holder<int>>) == sizeof(int);

template <bool = EBO_is_supported, typename EmptyA = A, typename EmptyB = B>
struct test {
    static_assert((sizeof(hana::tuple<int, EmptyA>) == sizeof(int)), "");
    static_assert((sizeof(hana::tuple<EmptyA, int>) == sizeof(int)), "");
    static_assert((sizeof(hana::tuple<EmptyA, int, EmptyB>) == sizeof(int)), "");
    static_assert((sizeof(hana::tuple<EmptyA, EmptyB, int>) == sizeof(int)), "");
    static_assert((sizeof(hana::tuple<int, EmptyA, EmptyB>) == sizeof(int)), "");
};

template <>
struct test<false> {
    // Nothing to test
};

template struct test<>;

int main() { }
