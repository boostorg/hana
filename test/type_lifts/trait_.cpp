/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/sandbox/detail/is_valid.hpp>

#include <type_traits>
using namespace boost::hana;


struct x1 { }; struct x2 { }; struct x3 { };

template <template <typename ...> class f>
void test() {
    // make sure the types are good
    static_assert(std::is_same<decltype(trait_<f>()), f<>>{}, "");
    static_assert(std::is_same<decltype(trait_<f>(x1{})), f<x1>>{}, "");
    static_assert(std::is_same<decltype(trait_<f>(x1{}, x2{})), f<x1, x2>>{}, "");
    static_assert(std::is_same<decltype(trait_<f>(x1{}, x2{}, x3{})), f<x1, x2, x3>>{}, "");

    // make sure we can use it; we already made sure the return type was correct
    trait_<f>();
    trait_<f>(x1{});
    trait_<f>(x1{}, x2{});
    trait_<f>(x1{}, x2{}, x3{});
}

template <typename ...> struct f { };
template <typename ...> struct invalid;

template <typename x = void, typename y = void, typename z = void>
using alias = f<x, y, z>;

int main() {
    test<f>();

    // test<alias>();
    // BOOST_HANA_CONSTANT_ASSERT(!detail::is_valid(trait_<invalid>)(x1{}));
}
