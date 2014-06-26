/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include "minimal.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(zip() == list());

    BOOST_HANA_STATIC_ASSERT(zip(ilist<>) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(zip(ilist<0>) == minimal_list(ilist<0>));
    BOOST_HANA_STATIC_ASSERT(zip(ilist<0, 1>) == minimal_list(ilist<0>, ilist<1>));
    BOOST_HANA_STATIC_ASSERT(zip(ilist<0, 1, 2>) == minimal_list(ilist<0>, ilist<1>, ilist<2>));

    BOOST_HANA_STATIC_ASSERT(zip(ilist<>, ilist<>) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(zip(ilist<0>, ilist<1>) == minimal_list(ilist<0, 1>));

    BOOST_HANA_STATIC_ASSERT(zip(ilist<0>, ilist<1>, ilist<2>) == minimal_list(ilist<0, 1, 2>));
    BOOST_HANA_STATIC_ASSERT(zip(ilist<0, 3>, ilist<1, 4>, ilist<2, 5, 8>) == minimal_list(ilist<0, 1, 2>, ilist<3, 4, 5>));
}
