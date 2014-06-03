/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(compose(list, list)(int_<0>) == list(list(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(compose(list, list)(int_<0>, int_<1>) == list(list(int_<0>), int_<1>));
    BOOST_HANA_STATIC_ASSERT(compose(list, list)(int_<0>, int_<1>, int_<2>) == list(list(int_<0>), int_<1>, int_<2>));
}
