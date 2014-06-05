/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2;

int main() {
    BOOST_HANA_STATIC_ASSERT(length(list_t<>) == size_t<0>);
    BOOST_HANA_STATIC_ASSERT(length(list_t<x0>) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(length(list_t<x0, x1>) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(length(list_t<x0, x1, x2>) == size_t<3>);
}
