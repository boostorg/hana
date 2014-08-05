/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integer_list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename T, typename U>
void test() {
    BOOST_HANA_CONSTANT_ASSERT(cons(integral<T, 0>, integer_list<U>) == integer_list<U, 0>);
    BOOST_HANA_CONSTANT_ASSERT(cons(integral<T, 0>, integer_list<U, 1>) == integer_list<U, 0, 1>);
    BOOST_HANA_CONSTANT_ASSERT(cons(integral<T, 0>, integer_list<U, 1, 2>) == integer_list<U, 0, 1, 2>);
    BOOST_HANA_CONSTANT_ASSERT(cons(integral<T, 0>, integer_list<U, 1, 2, 3>) == integer_list<U, 0, 1, 2, 3>);
}

int main() {
    test<int, int>();
    test<int, unsigned int>();

    test<int, long>();
    test<int, unsigned long>();
}
