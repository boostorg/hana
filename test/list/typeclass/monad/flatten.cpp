/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "../minimal.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(flatten(minimal_list(minimal_list(), minimal_list())) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(flatten(minimal_list(minimal_list(int_<0>), minimal_list())) == minimal_list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(flatten(minimal_list(minimal_list(), minimal_list(int_<0>))) == minimal_list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(flatten(minimal_list(minimal_list(int_<0>), minimal_list(int_<1>))) == minimal_list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(
        flatten(minimal_list(minimal_list(0, int_<1>), minimal_list(), minimal_list('2', 3.3), minimal_list(int_<4>)))
        ==
        minimal_list(0, int_<1>, '2', 3.3, int_<4>)
    );
}
