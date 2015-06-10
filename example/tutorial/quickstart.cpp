/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

// Make sure `assert` always triggers an assertion
#ifdef NDEBUG
#   undef NDEBUG
#endif

#include <cassert>
#include <iostream>
#include <string>

#include <boost/hana/ext/std/type_traits.hpp>

//! [includes]
#include <boost/hana.hpp>
using namespace boost::hana;
//! [includes]
using namespace boost::hana::literals;


int main() {

//! [animals]
struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

auto animals = make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});
//! [animals]

//! [algorithms]
// Access tuple elements with operator[] instead of std::get.
Cat garfield = animals[1_c];

// Perform high level algorithms on tuples (this is like std::transform)
auto names = transform(animals, [](auto a) {
  return a.name;
});

assert(reverse(names) == make_tuple("Snoopy", "Garfield", "Nemo"));
//! [algorithms]


//! [type-level]
auto animal_types = make_tuple(type<Fish*>, type<Cat&>, type<Dog>);

auto no_pointers = remove_if(animal_types, [](auto a) {
  return traits::is_pointer(a);
});

static_assert(no_pointers == make_tuple(type<Cat&>, type<Dog>), "");
//! [type-level]


//! [has_name]
auto has_name = is_valid([](auto&& x) -> decltype((void)x.name) { });

static_assert(has_name(garfield), "");
static_assert(!has_name(1), "");
//! [has_name]

#if 0
//! [screw_up]
auto serialize = [](std::ostream& os, auto const& object) {
  for_each(os, [&](auto member) {
    //     ^^ oopsie daisy!
    os << member << std::endl;
  });
};
//! [screw_up]
#endif

//! [serialization]
// 1. Give introspection capabilities to 'Person'
struct Person {
  BOOST_HANA_DEFINE_STRUCT(Person,
    (std::string, name),
    (int, age)
  );
};

// 2. Write a generic serializer (bear with std::ostream for the example)
auto serialize = [](std::ostream& os, auto const& object) {
  for_each(members(object), [&](auto member) {
    os << member << std::endl;
  });
};

// 3. Use it
Person john{"John", 30};
serialize(std::cout, john);

// output:
// John
// 30
//! [serialization]

}
