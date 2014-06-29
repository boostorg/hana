/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(less(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(not_(less(int_<0>, int_<0>)));
    BOOST_HANA_STATIC_ASSERT(not_(less(int_<1>, int_<0>)));

    BOOST_HANA_STATIC_ASSERT(less(int_<0>, long_<1>));
    BOOST_HANA_STATIC_ASSERT(not_(less(int_<0>, long_<0>)));
    BOOST_HANA_STATIC_ASSERT(not_(less(int_<1>, long_<0>)));
}
