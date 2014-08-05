/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type_list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2;

BOOST_HANA_CONSTANT_ASSERT(head(type_list<x0>) == type<x0>);
BOOST_HANA_CONSTANT_ASSERT(head(type_list<x0, x1>) == type<x0>);
BOOST_HANA_CONSTANT_ASSERT(head(type_list<x0, x1, x2>) == type<x0>);

int main() { }
