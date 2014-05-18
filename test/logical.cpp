/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


void test_if() {
    BOOST_HANA_STATIC_ASSERT(if_(true_, int_<0>, int_<1>) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(if_(false_, int_<0>, int_<1>) == int_<1>);
}

void test_and() {
    BOOST_HANA_STATIC_ASSERT(and_());
    BOOST_HANA_STATIC_ASSERT(and_(true_));
    BOOST_HANA_STATIC_ASSERT(and_(true_, true_));

    BOOST_HANA_STATIC_ASSERT(!and_(false_));
    BOOST_HANA_STATIC_ASSERT(!and_(true_, false_));
    BOOST_HANA_STATIC_ASSERT(!and_(true_, true_, false_));
    BOOST_HANA_STATIC_ASSERT(!and_(false_, true_, true_));
}

void test_or() {
    BOOST_HANA_STATIC_ASSERT(!or_());
    BOOST_HANA_STATIC_ASSERT(!or_(false_));
    BOOST_HANA_STATIC_ASSERT(!or_(false_, false_));

    BOOST_HANA_STATIC_ASSERT(or_(true_));
    BOOST_HANA_STATIC_ASSERT(or_(false_, true_));
    BOOST_HANA_STATIC_ASSERT(or_(false_, false_, true_));
    BOOST_HANA_STATIC_ASSERT(or_(true_, false_, false_));
}

int main() {
    test_if();
    test_and();
    test_or();
}
