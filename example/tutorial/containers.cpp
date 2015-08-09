/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
namespace hana = boost::hana;
using namespace hana::literals;
using namespace std::literals;


int main() {

{

//! [make<Tuple>]
auto xs = hana::make<hana::Tuple>(1, 2.2, 'a', "bcde"s);
//! [make<Tuple>]

}{

//! [make<Range>]
constexpr auto r = hana::make<hana::Range>(hana::int_<3>, hana::int_<10>);
static_assert(r == hana::make_range(hana::int_<3>, hana::int_<10>), "");
//! [make<Range>]

}{

//! [lifetime]
std::string hello = "Hello";
std::vector<char> world = {'W', 'o', 'r', 'l', 'd'};

// hello is copied, world is moved-in
auto xs = hana::make_tuple(hello, std::move(world));

// s is a reference to the copy of hello inside xs.
// It becomes a dangling reference as soon as xs is destroyed.
std::string& s = xs[0_c];
//! [lifetime]
(void)s;

}{

//! [reference_wrapper]
std::vector<int> ints = { /* huge vector of ints */ };
std::vector<std::string> strings = { /* huge vector of strings */ };

auto map = hana::make_map(
  hana::make_pair(hana::type<int>, std::ref(ints)),
  hana::make_pair(hana::type<std::string>, std::ref(strings))
);

auto& v = map[hana::type<int>].get();
BOOST_HANA_RUNTIME_CHECK(&v == &ints);
//! [reference_wrapper]

}{

//! [types]
auto xs = hana::make_tuple(1, '2', "345");
auto ints = hana::make_range(hana::int_<0>, hana::int_<100>);
// what can we say about the types of xs and ints?
//! [types]
(void)xs;
(void)ints;

}

}


namespace overloading {
//! [overloading]
template <typename T>
void f(std::vector<T> xs) {
  // ...
}

template <typename Xs, typename = std::enable_if_t<hana::is_a<hana::Tuple, Xs>()>>
void f(Xs xs) {
  // ...
}
//! [overloading]
}
