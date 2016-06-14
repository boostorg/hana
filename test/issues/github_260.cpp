// Copyright Jason Rice 2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include<boost/hana/functional/partial.hpp>
#include<boost/hana/functional/reverse_partial.hpp>
#include<boost/hana/equal.hpp>
#include<boost/hana/type.hpp>
#include<boost/hana/integral_constant.hpp>
namespace hana = boost::hana;

template <typename T1, typename T2>
struct test_t { };
constexpr auto test = hana::template_<test_t>;

int main() {
  {
    using F = decltype(hana::partial(test, hana::int_c<1>));
    constexpr F f{};
    static_assert(f(hana::int_c<2>) == test(hana::int_c<1>, hana::int_c<2>), "");
  }
  {
    using F = decltype(hana::reverse_partial(test, hana::int_c<2>));
    constexpr F f{};
    static_assert(f(hana::int_c<1>) == test(hana::int_c<1>, hana::int_c<2>), "");
  }
}
