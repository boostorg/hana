/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto foldable = list; // minimal foldable

void test_minimum() {
    BOOST_HANA_STATIC_ASSERT(minimum(foldable(int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(minimum(foldable(int_<0>, int_<1>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(minimum(foldable(int_<1>, int_<0>)) == int_<0>);
}

void test_maximum() {
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<0>, int_<1>)) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<1>, int_<0>)) == int_<1>);
}

void test_sum() {
    BOOST_HANA_STATIC_ASSERT(sum(foldable()) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(sum(foldable(int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(sum(foldable(int_<0>, int_<1>)) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(sum(foldable(int_<0>, int_<1>, int_<2>)) == int_<3>);
}

void test_product() {
    BOOST_HANA_STATIC_ASSERT(product(foldable()) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(product(foldable(int_<1>)) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(product(foldable(int_<1>, int_<2>)) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(product(foldable(int_<1>, int_<2>, int_<3>)) == int_<6>);
}

int main() {
    test_minimum();
    test_maximum();
    test_sum();
    test_product();
}
