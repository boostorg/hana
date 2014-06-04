/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(arg<1>(int_<1>) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(arg<1>(int_<1>, int_<2>) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(arg<1>(int_<1>, int_<2>, int_<3>) == int_<1>);

    BOOST_HANA_STATIC_ASSERT(arg<2>(int_<1>, int_<2>) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(arg<2>(int_<1>, int_<2>, int_<3>) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(arg<2>(1, '2', "3") == '2');

    BOOST_HANA_STATIC_ASSERT(arg<3>(int_<1>, int_<2>, int_<3>) == int_<3>);
}
