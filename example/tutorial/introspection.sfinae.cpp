/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <string>
#include <vector>
using namespace boost::hana;


struct yes { std::string toString() const { return "yes"; } };
struct no { };

//! [optionalToString.sfinae]
template <typename T>
std::string optionalToString(T const& obj) {
  auto maybe_toString = sfinae([](auto&& x) -> decltype(x.toString()) {
    return x.toString();
  });

  return from_maybe("toString not defined", maybe_toString(obj));
}
//! [optionalToString.sfinae]

int main() {
BOOST_HANA_RUNTIME_CHECK(optionalToString(yes{}) == "yes");
BOOST_HANA_RUNTIME_CHECK(optionalToString(no{}) == "toString not defined");

{

//! [maybe_add]
auto maybe_add = sfinae([](auto x, auto y) -> decltype(x + y) {
  return x + y;
});

maybe_add(1, 2); // just(3)

std::vector<int> v;
maybe_add(v, "foobar"); // nothing
//! [maybe_add]

}
}
