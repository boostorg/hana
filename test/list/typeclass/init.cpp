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
    BOOST_HANA_STATIC_ASSERT(init(minimal_list(0)) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(init(minimal_list(0, '1')) == minimal_list(0));
    BOOST_HANA_STATIC_ASSERT(init(minimal_list(0, '1', 2.2)) == minimal_list(0, '1'));
    BOOST_HANA_STATIC_ASSERT(init(minimal_list(0, '1', 2.2, nullptr)) == minimal_list(0, '1', 2.2));
}
