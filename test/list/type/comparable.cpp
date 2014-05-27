/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2;

int main() {
    BOOST_HANA_STATIC_ASSERT(list_t<> == list_t<>);
    BOOST_HANA_STATIC_ASSERT(list_t<x0> != list_t<>);
    BOOST_HANA_STATIC_ASSERT(list_t<> != list_t<x0>);
    BOOST_HANA_STATIC_ASSERT(list_t<x0> == list_t<x0>);
    BOOST_HANA_STATIC_ASSERT(list_t<x0, x1> != list_t<x0>);
    BOOST_HANA_STATIC_ASSERT(list_t<x0> != list_t<x0, x1>);
    BOOST_HANA_STATIC_ASSERT(list_t<x0, x1> == list_t<x0, x1>);
    BOOST_HANA_STATIC_ASSERT(list_t<x0, x1, x2> == list_t<x0, x1, x2>);
}
