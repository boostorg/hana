/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

// Make sure `assert` always triggers an assertion
#ifdef NDEBUG
#   undef NDEBUG
#endif

#include <boost/any.hpp>
#include <cassert>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>

//! [includes]
#include <boost/hana.hpp>
using namespace boost::hana;
//! [includes]


//////////////////////////////////////////////////////////////////////////////
// IMPORTANT:
// Any change in this file must be acommpanied by matching changes in the
// quickstart section of the tutorial.
//////////////////////////////////////////////////////////////////////////////

//! [full]
//! [cases]
template <typename T>
auto case_ = [](auto f) {
  return std::make_pair(type<T>, f);
};

struct _default;
auto default_ = case_<_default>;
//! [cases]

//! [process]
template <typename Any, typename Default>
auto process(Any&, std::type_index const&, Default& default_) {
  return default_();
}

template <typename Any, typename Default, typename Case, typename ...Rest>
auto process(Any& a, std::type_index const& t, Default& default_,
             Case& case_, Rest& ...rest)
{
  using T = typename decltype(case_.first)::type;
  return t == typeid(T) ? case_.second(*boost::unsafe_any_cast<T>(&a))
                        : process(a, t, default_, rest...);
}
//! [process]

//! [switch_]
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = make_tuple(cases_...);

    auto default_ = find_if(cases, [](auto const& c) {
      return c.first == type<_default>;
    });
    static_assert(default_ != nothing,
      "switch is missing a default_ case");

    auto rest = filter(cases, [](auto const& c) {
      return c.first != type<_default>;
    });

    return unpack(rest, [&](auto& ...rest) {
      return process(a, a.type(), default_->second, rest...);
    });
  };
}
//! [switch_]
//! [full]


int main() {
using namespace std::literals;
using ::default_; // disambiguate with boost::hana::default_

{

//! [usage]
boost::any a = 'x';
std::string r = switch_(a)(
  case_<int>([](auto i) { return "int: "s + std::to_string(i); }),
  case_<char>([](auto c) { return "char: "s + std::string{c}; }),
  default_([] { return "unknown"s; })
);

assert(r == "char: x"s);
//! [usage]

}{

//! [result_inference]
boost::any a = 'x';
auto r = switch_(a)(
  case_<int>([](auto) -> int { return 1; }),
  case_<char>([](auto) -> long { return 2l; }),
  default_([]() -> long long { return 3ll; })
);

// r is inferred to be a long long
static_assert(std::is_same<decltype(r), long long>{}, "");
assert(r == 2ll);
//! [result_inference]

}

}
