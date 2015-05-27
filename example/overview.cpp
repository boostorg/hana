/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

// Make sure assert always triggers an assertion
#ifdef NDEBUG
#   undef NDEBUG
#endif

//////////////////////////////////////////////////////////////////////////////
// Important: Keep this file in sync with the Overview in the README
//////////////////////////////////////////////////////////////////////////////
#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>
#include <cassert>
#include <string>
using namespace boost::hana;
using namespace boost::hana::literals;

struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

int main() {
  // Sequences capable of holding heterogeneous objects, and algorithms
  // to manipulate them.
  auto animals = make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});
  auto names = transform(animals, [](auto a) {
    return a.name;
  });
  assert(reverse(names) == make_tuple("Snoopy", "Garfield", "Nemo"));

  // No compile-time information is lost: even if `animals` can't be a
  // constant expression because it contains strings, its length is constexpr.
  static_assert(length(animals) == 3u, "");

  // Computations on types can be performed with the same syntax as that of
  // normal C++. Believe it or not, everything is done at compile-time.
  auto animal_types = make_tuple(type<Fish*>, type<Cat&>, type<Dog*>);
  auto animal_ptrs = filter(animal_types, [](auto a) {
    return traits::is_pointer(a);
  });
  static_assert(animal_ptrs == make_tuple(type<Fish*>, type<Dog*>), "");

  // And many other goodies to make your life easier, including:
  // 1. Access to elements in a tuple with a sane syntax.
  static_assert(animal_ptrs[0_c] == type<Fish*>, "");
  static_assert(animal_ptrs[1_c] == type<Dog*>, "");

  // 2. Unroll loops at compile-time without hassle.
  std::string s;
  int_<10>.times([&]{ s += "x"; });
  // equivalent to s += "x"; s += "x"; ... s += "x";

  // 3. Easily check whether an expression is valid.
  //    This is usually achieved with complex SFINAE-based tricks.
  auto has_name = is_valid([](auto&& x) -> decltype(x.name) { });
  static_assert(!has_name(type<int>), "");
  static_assert(has_name(type<Cat>), "");
}