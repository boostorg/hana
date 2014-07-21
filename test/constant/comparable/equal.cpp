/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


struct UDT {
    struct Type1 { };
    struct Type2 { };
    Type1 member1;
    Type2 member2;
};

template <typename T, typename U>
void test() {
    BOOST_HANA_STATIC_ASSERT( equal(constant<T, 0>, constant<U, 0>));
    BOOST_HANA_STATIC_ASSERT(!equal(constant<T, 0>, constant<U, 1>));
    // Incomparable constants must appear as unequal.
    BOOST_HANA_STATIC_ASSERT(!equal(
        constant<T, 0>,
        constant<decltype(&UDT::member1), &UDT::member1>
    ));
    BOOST_HANA_STATIC_ASSERT(!equal(
        constant<decltype(&UDT::member1), &UDT::member1>,
        constant<decltype(&UDT::member2), &UDT::member2>
    ));
}

int main() {
    test<int, int>();
    test<int, long>();
}
