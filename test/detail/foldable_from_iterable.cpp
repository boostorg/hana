/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/foldable_from_iterable.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto iterable = list; // minimal iterable

void test_foldl() {
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, iterable()) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, iterable(int_<1>)) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, iterable(int_<1>, int_<2>)) == list(list(int_<0>, int_<1>), int_<2>));
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, iterable(int_<1>, int_<2>, int_<3>)) == list(list(list(int_<0>, int_<1>), int_<2>), int_<3>));
}

void test_foldl1() {
    BOOST_HANA_STATIC_ASSERT(foldl1(list, iterable(int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldl1(list, iterable(int_<0>, int_<1>)) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldl1(list, iterable(int_<0>, int_<1>, int_<2>)) == list(list(int_<0>, int_<1>), int_<2>));
    BOOST_HANA_STATIC_ASSERT(foldl1(list, iterable(int_<0>, int_<1>, int_<2>, int_<3>)) == list(list(list(int_<0>, int_<1>), int_<2>), int_<3>));
}

void test_foldr() {
    BOOST_HANA_STATIC_ASSERT(foldr(list, int_<0>, iterable()) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldr(list, int_<1>, iterable(int_<0>)) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldr(list, int_<2>, iterable(int_<0>, int_<1>)) == list(int_<0>, list(int_<1>, int_<2>)));
    BOOST_HANA_STATIC_ASSERT(foldr(list, int_<3>, iterable(int_<0>, int_<1>, int_<2>)) == list(int_<0>, list(int_<1>, list(int_<2>, int_<3>))));
}

void test_foldr1() {
    BOOST_HANA_STATIC_ASSERT(foldr1(list, iterable(int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldr1(list, iterable(int_<0>, int_<1>)) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldr1(list, iterable(int_<0>, int_<1>, int_<2>)) == list(int_<0>, list(int_<1>, int_<2>)));
    BOOST_HANA_STATIC_ASSERT(foldr1(list, iterable(int_<0>, int_<1>, int_<2>, int_<3>)) == list(int_<0>, list(int_<1>, list(int_<2>, int_<3>))));
}

int main() {
    test_foldl();
    test_foldl1();
    test_foldr();
    test_foldr1();
}
