/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


template <typename T>
constexpr void test_for() {
    BOOST_HANA_CONSTANT_ASSERT(equal(
        decltype_(integral<T const, 0>),
        decltype_(integral<T, 0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        decltype_(integral<T volatile, 0>),
        decltype_(integral<T, 0>)
    ));
    BOOST_HANA_CONSTANT_ASSERT(equal(
        decltype_(integral<T const volatile, 0>),
        decltype_(integral<T, 0>)
    ));
}

int main() {
    test_for<bool>();
    test_for<int>();
    test_for<unsigned int>();
    test_for<char>();
    test_for<unsigned long long>();
}
