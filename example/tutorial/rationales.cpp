/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <type_traits>
using namespace boost::hana;


namespace index_based_recursion {
template <typename ...T>
constexpr void use(T&& ...) { }

//! [index_based_recursion]
template <typename Xs, typename Index = decltype(size_t<0>)>
auto f(Xs&& xs, Index i = size_t<0>) {
  auto next = if_(i != length(xs),
    [](auto&& xs, auto i) {
      use(xs[i]);
      return f(xs, i + size_t<1>);
    },
    [](auto&& xs, auto i) {
      // base case
    }
  );

  return next(xs, i);
}
//! [index_based_recursion]
}

namespace constrained_index_based_recursion {
//! [constrained_index_based_recursion]
template <typename Xs, typename Index = decltype(size_t<0>),
          typename = std::enable_if_t<is_a<Tuple, Xs>()>>
auto f(Xs&& xs, Index i = size_t<0>) {
  // same as before
}
//! [constrained_index_based_recursion]
}

int main() {
  // just make sure they compile
  index_based_recursion::f(make_tuple(1, 2, 3));
  constrained_index_based_recursion::f(make_tuple(1, 2, 3));
}
