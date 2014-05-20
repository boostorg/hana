/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto iterable = list; // minimal iterable

void test_at() {
    BOOST_HANA_STATIC_ASSERT(at(int_<0>, iterable(int_<0>)) == int_<0>);

    BOOST_HANA_STATIC_ASSERT(at(int_<0>, iterable(int_<0>, int_<1>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(at(int_<1>, iterable(int_<0>, int_<1>)) == int_<1>);

    BOOST_HANA_STATIC_ASSERT(at(int_<0>, iterable(int_<0>, int_<1>, int_<2>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(at(int_<1>, iterable(int_<0>, int_<1>, int_<2>)) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(at(int_<2>, iterable(int_<0>, int_<1>, int_<2>)) == int_<2>);
}

void test_last() {
    BOOST_HANA_STATIC_ASSERT(last(iterable(int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(last(iterable(int_<0>, int_<1>)) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(last(iterable(int_<0>, int_<1>, int_<2>)) == int_<2>);
}

void test_length() {
    BOOST_HANA_STATIC_ASSERT(length(iterable()) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(length(iterable(int_<0>)) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(length(iterable(int_<0>, int_<1>)) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(length(iterable(int_<0>, int_<1>, int_<2>)) == int_<3>);
}

void test_drop() {
    BOOST_HANA_STATIC_ASSERT(drop(int_<0>, iterable()) == iterable());
    BOOST_HANA_STATIC_ASSERT(drop(int_<1>, iterable()) == iterable());
    BOOST_HANA_STATIC_ASSERT(drop(int_<2>, iterable()) == iterable());

    BOOST_HANA_STATIC_ASSERT(drop(int_<0>, iterable(int_<0>)) == iterable(int_<0>));
    BOOST_HANA_STATIC_ASSERT(drop(int_<1>, iterable(int_<0>)) == iterable());
    BOOST_HANA_STATIC_ASSERT(drop(int_<2>, iterable(int_<0>)) == iterable());

    BOOST_HANA_STATIC_ASSERT(drop(int_<0>, iterable(int_<0>, int_<1>)) == iterable(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(drop(int_<1>, iterable(int_<0>, int_<1>)) == iterable(int_<1>));
    BOOST_HANA_STATIC_ASSERT(drop(int_<2>, iterable(int_<0>, int_<1>)) == iterable());
}

void test_all_of() {
    BOOST_HANA_STATIC_ASSERT(all_of(iterable()));
    BOOST_HANA_STATIC_ASSERT(all_of(iterable(true_)));
    BOOST_HANA_STATIC_ASSERT(all_of(iterable(true_, true_)));
    BOOST_HANA_STATIC_ASSERT(!all_of(iterable(true_, true_, false_)));
    BOOST_HANA_STATIC_ASSERT(!all_of(iterable(false_, true_, true_)));

    BOOST_HANA_STATIC_ASSERT(all_of(iterable(true, true)));
    BOOST_HANA_STATIC_ASSERT(!all_of(iterable(true, true, false)));
}

int main() {
    test_at();
    test_last();
    test_length();
    test_drop();
    test_all_of();
}
