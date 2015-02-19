/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifdef NDEBUG
#   undef NDEBUG
#endif

//////////////////////////////////////////////////////////////////////////////
// Important: Keep this file in sync with the Overview in the README
//////////////////////////////////////////////////////////////////////////////

#include <boost/hana.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
using namespace boost::hana;
using namespace boost::hana::literals;


struct Person { std::string name; };
struct Car    { std::string name; };
struct City   { std::string name; };

int main() {
    // Sequences holding heterogeneous objects.
    auto stuff = tuple(Person{"Louis"}, Car{"Toyota"}, City{"Quebec"});

    // Expressive algorithms to manipulate them.
    auto names = transform(stuff, [](auto x) { return x.name; });
    assert(reverse(names) == tuple("Quebec", "Toyota", "Louis"));

    // No compile-time information is lost: even if `stuff` can't be constexpr
    // because it holds `std::string`s, its length is known at compile-time.
    static_assert(length(stuff) == 3u, "");

    // A way to represent types as values and to "lift" metafunctions into
    // normal functions, so everything you can do with the MPL can be done
    // with Hana, using a sane syntax.
    constexpr auto types = tuple(type<Car>, type<City>, type<void>);
    constexpr auto pointers = transform(types, metafunction<std::add_pointer>);
    static_assert(pointers == tuple(type<Car*>, type<City*>, type<void*>), "");

    // And many other goodies to make your life better.
    static_assert(10_c == std::integral_constant<long long, 10>{}, "");

    static_assert(pointers[1_c] == type<City*>, "std::get, revisited");

    int_<10>.times([]{
        std::cout << "rubyists rejoice!" << std::endl;
    });
}
