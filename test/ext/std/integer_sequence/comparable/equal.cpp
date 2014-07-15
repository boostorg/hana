/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integer_sequence.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <utility>
using namespace boost::hana;


template <typename T, typename U>
void test() {
    BOOST_HANA_STATIC_ASSERT(equal(std::integer_sequence<T>{}, std::integer_sequence<U>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(std::integer_sequence<T, 0>{}, std::integer_sequence<U>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(std::integer_sequence<T>{}, std::integer_sequence<U, 0>{}));

    BOOST_HANA_STATIC_ASSERT(equal(std::integer_sequence<T, 0>{}, std::integer_sequence<U, 0>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(std::integer_sequence<T, 0>{}, std::integer_sequence<U, 0, 1>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(std::integer_sequence<T, 0, 2>{}, std::integer_sequence<U, 0, 1>{}));

    BOOST_HANA_STATIC_ASSERT(equal(std::integer_sequence<T, 0, 1, 2, 3>{}, std::integer_sequence<U, 0, 1, 2, 3>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(std::integer_sequence<T, 0, 1, 2, 3, 5>{}, std::integer_sequence<U, 0, 1, 2, 3>{}));
}

template <typename T>
void test_partial() {
    test<T, char>();
    test<T, short>();
    test<T, int>();
    test<T, long>();
    test<T, long long>();

    test<T, unsigned short>();
    test<T, unsigned int>();
    test<T, unsigned long>();
    test<T, unsigned long long>();
}

int main() {
    test_partial<char>();
    test_partial<short>();
    test_partial<int>();
    test_partial<long>();
    test_partial<long long>();

    test_partial<unsigned short>();
    test_partial<unsigned int>();
    test_partial<unsigned long>();
    test_partial<unsigned long long>();
}
