/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(std::integral_constant<int, 1>{} == int_<1>);
    BOOST_HANA_STATIC_ASSERT(std::integral_constant<int, 1>{} == long_<1>);

    BOOST_HANA_STATIC_ASSERT(std::integral_constant<int, 2>{} != int_<3>);
}
