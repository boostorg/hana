/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename T>
void test() {
    BOOST_HANA_CONSTANT_ASSERT(range_c<T, 0, 0> == range(integral<T, 0>, integral<T, 0>));
    BOOST_HANA_CONSTANT_ASSERT(range_c<T, 0, 1> == range(integral<T, 0>, integral<T, 1>));
    BOOST_HANA_CONSTANT_ASSERT(range_c<T, 0, 2> == range(integral<T, 0>, integral<T, 2>));

    BOOST_HANA_CONSTANT_ASSERT(range_c<T, 1, 1> == range(integral<T, 1>, integral<T, 1>));
    BOOST_HANA_CONSTANT_ASSERT(range_c<T, 1, 2> == range(integral<T, 1>, integral<T, 2>));
    BOOST_HANA_CONSTANT_ASSERT(range_c<T, 1, 3> == range(integral<T, 1>, integral<T, 3>));
}

int main() {
    test<int>();
    test<long>();
    test<unsigned long>();
    test<unsigned long long>();
}
