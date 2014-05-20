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
    BOOST_HANA_STATIC_ASSERT(list() == list());
    BOOST_HANA_STATIC_ASSERT(list(int_<0>) != list());
    BOOST_HANA_STATIC_ASSERT(list() != list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(list(int_<0>) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(list(int_<0>, int_<1>) != list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(list(int_<0>, int_<1>) == list(int_<0>, int_<1>));

    BOOST_HANA_STATIC_ASSERT(list(0) == list(0));
    BOOST_HANA_STATIC_ASSERT(list(0, 1) != list(0, 2));
    BOOST_HANA_STATIC_ASSERT(list(0, 1, 2) == list(0, 1, 2));
}
