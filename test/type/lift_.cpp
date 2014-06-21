/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename ...> struct f { };
struct x1 { }; struct x2 { }; struct x3 { };

int main() {
    // make sure the types are good
    static_assert(std::is_same<decltype(lift_<f>()), f<>>{}, "");
    static_assert(std::is_same<decltype(lift_<f>(x1{})), f<x1>>{}, "");
    static_assert(std::is_same<decltype(lift_<f>(x1{}, x2{})), f<x1, x2>>{}, "");
    static_assert(std::is_same<decltype(lift_<f>(x1{}, x2{}, x3{})), f<x1, x2, x3>>{}, "");

    // make sure we can use it; we already made sure the return type was correct
    lift_<f>();
    lift_<f>(x1{});
    lift_<f>(x1{}, x2{});
    lift_<f>(x1{}, x2{}, x3{});
}
