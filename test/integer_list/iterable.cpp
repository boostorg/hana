/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integer_list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    // head
    {
        BOOST_HANA_CONSTANT_ASSERT(head(integer_list<int, 0>) == int_<0>);
        BOOST_HANA_CONSTANT_ASSERT(head(integer_list<int, 0, 1>) == int_<0>);
        BOOST_HANA_CONSTANT_ASSERT(head(integer_list<int, 0, 1, 2>) == int_<0>);
    }

    // is_empty
    {
        BOOST_HANA_CONSTANT_ASSERT(is_empty(integer_list<int>));
        BOOST_HANA_CONSTANT_ASSERT(!is_empty(integer_list<int, 0>));
        BOOST_HANA_CONSTANT_ASSERT(!is_empty(integer_list<int, 0, 1>));
        BOOST_HANA_CONSTANT_ASSERT(!is_empty(integer_list<int, 0, 1, 2>));
    }

    // tail
    {
        BOOST_HANA_CONSTANT_ASSERT(tail(integer_list<int, 0>) == integer_list<int>);
        BOOST_HANA_CONSTANT_ASSERT(tail(integer_list<int, 0, 1>) == integer_list<int, 1>);
        BOOST_HANA_CONSTANT_ASSERT(tail(integer_list<int, 0, 1, 2>) == integer_list<int, 1, 2>);
    }
}
