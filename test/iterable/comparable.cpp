/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "minimal_iterable.hpp"
using namespace boost::hana;


template <int i, int j>
void test() {
    auto iter1 = minimal_iterable<i>;
    auto iter2 = minimal_iterable<j>;
    BOOST_HANA_STATIC_ASSERT(equal(iter1(), iter2()));
    BOOST_HANA_STATIC_ASSERT(not_equal(iter1(int_<0>), iter2()));
    BOOST_HANA_STATIC_ASSERT(not_equal(iter1(), iter2(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(equal(iter1(int_<0>), iter2(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(not_equal(iter1(int_<0>, int_<1>), iter2(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(equal(iter1(int_<0>, int_<1>), iter2(int_<0>, int_<1>)));

    BOOST_HANA_STATIC_ASSERT(equal(iter1(0), iter2(0)));
    BOOST_HANA_STATIC_ASSERT(not_equal(iter1(0, 1), iter2(0, 2)));
    BOOST_HANA_STATIC_ASSERT(equal(iter1(0, 1, 2), iter2(0, 1, 2)));
};

int main() {
    test<0, 0>();
    test<0, 1>();
}
