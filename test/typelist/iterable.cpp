/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/typelist.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2;

int main() {
    BOOST_HANA_STATIC_ASSERT(is_empty(typelist<>));
    BOOST_HANA_STATIC_ASSERT(!is_empty(typelist<x0>));
    BOOST_HANA_STATIC_ASSERT(!is_empty(typelist<x0, x1>));

    BOOST_HANA_STATIC_ASSERT(head(typelist<x0>) == type<x0>);
    BOOST_HANA_STATIC_ASSERT(head(typelist<x0, x1>) == type<x0>);
    BOOST_HANA_STATIC_ASSERT(head(typelist<x0, x1, x2>) == type<x0>);

    BOOST_HANA_STATIC_ASSERT(tail(typelist<x0>) == typelist<>);
    BOOST_HANA_STATIC_ASSERT(tail(typelist<x0, x1>) == typelist<x1>);
    BOOST_HANA_STATIC_ASSERT(tail(typelist<x0, x1, x2>) == typelist<x1, x2>);
}
