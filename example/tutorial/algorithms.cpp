/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>

#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;
using namespace std::literals;


int main() {

{

//! [reverse_transform]
auto to_str = [](auto const& x) {
  std::stringstream ss;
  ss << x;
  return ss.str();
};

auto xs = make_tuple(1, 2.2, 'a', "bcde");
BOOST_HANA_RUNTIME_CHECK(
  reverse(transform(xs, to_str)) == make_tuple("bcde", "a", "2.2", "1")
);
//! [reverse_transform]

//! [reverse_transform_copy]
reverse(
  transform(xs, to_str) // <-- copy into reverse(...) here?
);
//! [reverse_transform_copy]

//! [reverse_transform_move]
reverse(
  transform(xs, to_str) // <-- nope, move from the temporary instead!
);
//! [reverse_transform_move]

}{

//! [effects]
auto r = any_of(make_tuple("hello"s, 1.2, 3), [](auto x) {
  return std::is_integral<decltype(x)>{};
});
static_assert(r, "");
//! [effects]

}

}
