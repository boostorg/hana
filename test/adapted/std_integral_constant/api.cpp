/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_integral_constant.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
#include <type_traits>
using namespace boost::hana;


int main() {
    // operator()
    static_assert(std::integral_constant<std::size_t, 0>{}() == 0, "");
    static_assert(std::integral_constant<std::size_t, 1>{}() == 1, "");
    static_assert(std::integral_constant<int, -3>{}() == -3, "");

    // decltype(operator())
    BOOST_HANA_STATIC_ASSERT(decltype_(std::integral_constant<std::size_t, 0>{}()) == type<std::size_t>);
    BOOST_HANA_STATIC_ASSERT(decltype_(std::integral_constant<int, -3>{}()) == type<int>);

    // conversions
    constexpr std::size_t a = std::integral_constant<std::size_t, 0>{}, b = std::integral_constant<std::size_t, 1>{};
    static_assert(a == 0 && b == 1, "");

    constexpr int c = std::integral_constant<int, 0>{}, d = std::integral_constant<int, -3>{};
    static_assert(c == 0 && d == -3, "");
}
