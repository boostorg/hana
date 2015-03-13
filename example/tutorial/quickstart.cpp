/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

// Make sure assert always triggers an assertion
#ifdef NDEBUG
#   undef NDEBUG
#endif

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <boost/hana/ext/std/type_traits.hpp>

//! [includes]
#include <boost/hana.hpp>
using namespace boost::hana;
//! [includes]


int main() {

// IMPORTANT NOTE:
// When changing this, make sure the corresponding changes are propagated
// to `example/tutorial/amphi.cpp`.

//! [decls]
struct Person { std::string name; };
struct Car    { std::string name; };
struct City   { std::string name; };
//! [decls]

//! [make_tuple]
auto stuff = make_tuple(Person{"Louis"}, Car{"Toyota"}, City{"Quebec"});
//! [make_tuple]

{
//! [_tuple]
_tuple<Person, Car, City> stuff{Person{"Louis"}, Car{"Toyota"}, City{"Quebec"}};
//! [_tuple]
}

//! [basic_operations]
Car& car = at_c<1>(stuff);
assert(car.name == "Toyota");
static_assert(length(stuff) == 3u, "");
//! [basic_operations]

//! [transform]
_tuple<std::string, std::string, std::string> names = transform(stuff, [](auto x) { return x.name; });
assert(names == make_tuple("Louis", "Toyota", "Quebec"));
//! [transform]

//! [metafunction]
using namespace traits; // bring in remove_cv and add_pointer

auto F = [](auto T) { // F is a "metafunction"; just pretend T is a type
    return add_pointer(remove_cv(T));
};
//! [metafunction]

//! [type]
auto intp = F(type<int const>);
static_assert(intp == type<int*>, "");
//! [type]

}
