/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/maybe.hpp>

#include "minimal_iterable.hpp"
using namespace boost::hana;


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

BOOST_HANA_CONSTEXPR_LAMBDA auto strict_list = on(list, apply);

void test_lazy_foldr() {
    BOOST_HANA_STATIC_ASSERT(lazy_foldr(strict_list, int_<0>, iterable()) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(lazy_foldr(strict_list, int_<1>, iterable(int_<0>)) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(lazy_foldr(strict_list, int_<2>, iterable(int_<0>, int_<1>)) == list(int_<0>, list(int_<1>, int_<2>)));
    BOOST_HANA_STATIC_ASSERT(lazy_foldr(strict_list, int_<3>, iterable(int_<0>, int_<1>, int_<2>)) == list(int_<0>, list(int_<1>, list(int_<2>, int_<3>))));
}

void test_find() {
    BOOST_HANA_STATIC_ASSERT(find(always(true_), iterable()) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(always(true_), iterable(int_<0>)) == just(int_<0>));
    BOOST_HANA_STATIC_ASSERT(find(always(false_), iterable(int_<0>)) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(id, iterable(true_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(true_, false_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(false_, true_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(false_, false_)) == nothing);
}

int main() {
    test_foldl();
    test_foldl1();
    test_foldr();
    test_foldr1();
    test_lazy_foldr();
    test_find();
}
