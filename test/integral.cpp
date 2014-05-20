/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


void test_Comparable() {
    BOOST_HANA_STATIC_ASSERT(equal(int_<0>, int_<0>));
    BOOST_HANA_STATIC_ASSERT(!equal(int_<1>, int_<0>));


    BOOST_HANA_STATIC_ASSERT(not_equal(int_<1>, int_<0>));
    BOOST_HANA_STATIC_ASSERT(!not_equal(int_<0>, int_<0>));
}

int main() {
    test_Comparable();

    // Arithmetic
    BOOST_HANA_STATIC_ASSERT(+int_<1> == int_<1>);
    BOOST_HANA_STATIC_ASSERT(-int_<1> == int_<-1>);
    BOOST_HANA_STATIC_ASSERT(int_<1> + int_<2> == int_<3>);
    BOOST_HANA_STATIC_ASSERT(int_<1> - int_<2> == int_<-1>);
    BOOST_HANA_STATIC_ASSERT(int_<3> * int_<2> == int_<6>);
    BOOST_HANA_STATIC_ASSERT(int_<6> / int_<3> == int_<2>);
    BOOST_HANA_STATIC_ASSERT(int_<6> % int_<4> == int_<2>);
    BOOST_HANA_STATIC_ASSERT(~int_<6> == int_<~6>);
    BOOST_HANA_STATIC_ASSERT((int_<6> & int_<3>) == int_<6 & 3>);
    BOOST_HANA_STATIC_ASSERT(int_<6> | int_<3> == int_<6 | 3>);
    BOOST_HANA_STATIC_ASSERT(int_<6> ^ int_<3> == int_<6 ^ 3>);
    BOOST_HANA_STATIC_ASSERT((int_<6> << int_<3>) == int_<(6 << 3)>);
    BOOST_HANA_STATIC_ASSERT((int_<6> >> int_<3>) == int_<(6 >> 3)>);

    // Comparison
    BOOST_HANA_STATIC_ASSERT(int_<0> == int_<0>);
    BOOST_HANA_STATIC_ASSERT(int_<1> != int_<0>);
    BOOST_HANA_STATIC_ASSERT(int_<0> < int_<1>);
    BOOST_HANA_STATIC_ASSERT(int_<0> <= int_<1>);
    BOOST_HANA_STATIC_ASSERT(int_<0> <= int_<0>);
    BOOST_HANA_STATIC_ASSERT(int_<1> > int_<0>);
    BOOST_HANA_STATIC_ASSERT(int_<1> >= int_<0>);
    BOOST_HANA_STATIC_ASSERT(int_<0> >= int_<0>);

    // Logical
    BOOST_HANA_STATIC_ASSERT(int_<3> || int_<0>);
    BOOST_HANA_STATIC_ASSERT(int_<3> && int_<1>);
    BOOST_HANA_STATIC_ASSERT(!int_<0>);
    BOOST_HANA_STATIC_ASSERT(!!int_<3>);
}
