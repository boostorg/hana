/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/struct_macros.hpp>

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
using namespace boost::hana;
using namespace std::literals;




template <typename Xs>
std::string join(Xs&& xs, std::string sep) {
  return fold.left(intersperse(std::forward<Xs>(xs), sep), "", _ + _);
}

// sample(json-base)
std::string quote(std::string s) { return "\"" + s + "\""; }

template <typename T>
auto to_json(T const& x) -> decltype(std::to_string(x)) {
  return std::to_string(x);
}

std::string to_json(char c) { return quote({c}); }
std::string to_json(std::string s) { return quote(s); }
// end-sample

// sample(json-Struct)
template <typename T>
  std::enable_if_t<models<Struct, T>(),
std::string> to_json(T const& x) {
  auto json = transform(keys(x), [&](auto name) {
    auto const& member = at_key(x, name);
    return quote(to<char const*>(name)) + " : " + to_json(member);
  });

  return "{" + join(std::move(json), ", ") + "}";
}
// end-sample

// sample(json-Sequence)
template <typename Xs>
  std::enable_if_t<models<Sequence, Xs>(),
std::string> to_json(Xs const& xs) {
  auto json = transform(xs, [](auto const& x) {
    return to_json(x);
  });

  return "[" + join(std::move(json), ", ") + "]";
}
// end-sample


int main() {
// sample(json-usage)
struct Person {
  BOOST_HANA_DEFINE_STRUCT(Person,
    (std::string, name),
    (int, age)
  );
};

Person joe{"Joe", 30};
std::cout << to_json(make_tuple(1, 'c', joe));
// end-sample
}
