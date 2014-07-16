/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    // Integral == Integral
    BOOST_HANA_STATIC_ASSERT(equal(int_<0>, int_<0>));
    BOOST_HANA_STATIC_ASSERT(!equal(int_<1>, int_<0>));

    BOOST_HANA_STATIC_ASSERT(equal(int_<0>, long_<0>));
    BOOST_HANA_STATIC_ASSERT(!equal(int_<1>, long_<0>));


    // Integral == other
    BOOST_HANA_STATIC_ASSERT(equal(int_<0>, int{0}));
    BOOST_HANA_STATIC_ASSERT(!equal(int_<0>, int{1}));

    BOOST_HANA_STATIC_ASSERT(equal(int_<0>, long{0}));
    BOOST_HANA_STATIC_ASSERT(!equal(int_<1>, long{0}));


    // other == Integral
    BOOST_HANA_STATIC_ASSERT(equal(int{0}, int_<0>));
    BOOST_HANA_STATIC_ASSERT(!equal(int{1}, int_<0>));

    BOOST_HANA_STATIC_ASSERT(equal(long{0}, int_<0>));
    BOOST_HANA_STATIC_ASSERT(!equal(long{1}, int_<0>));
}
