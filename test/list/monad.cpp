/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(unit<List>(int_<0>) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(unit<List>(int_<1>) == list(int_<1>));
    BOOST_HANA_STATIC_ASSERT(unit<List>(3) == list(3));

    BOOST_HANA_STATIC_ASSERT(join(list(list(), list())) == list());
    BOOST_HANA_STATIC_ASSERT(join(list(list(int_<0>), list())) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(join(list(list(), list(int_<0>))) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(join(list(list(int_<0>), list(int_<1>))) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(
        join(list(list(0, int_<1>), list(), list('2', "3"), list(int_<4>)))
        ==
        list(0, int_<1>, '2', "3", int_<4>)
    );
}
