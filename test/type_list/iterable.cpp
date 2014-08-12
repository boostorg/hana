/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type_list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/type.hpp>

#include <test/laws/iterable.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2;

int main() {
    // head
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(head(type_list<x0>), type<x0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(head(type_list<x0, x1>), type<x0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(head(type_list<x0, x1, x2>), type<x0>));
    }

    // is_empty
    {
        BOOST_HANA_CONSTANT_ASSERT(is_empty(type_list<>));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(type_list<x0>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(type_list<x0, x1>)));
    }

    // tail
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(type_list<x0>), type_list<>));
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(type_list<x0, x1>), type_list<x1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(type_list<x0, x1, x2>), type_list<x1, x2>));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Iterable_laws(
            type_list<>,
            type_list<x0>,
            type_list<x0, x1>,
            type_list<x0, x1, x2>
        ));
    }
}
