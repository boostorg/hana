/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(cons(0, std::make_tuple()) == std::make_tuple(0));
    BOOST_HANA_STATIC_ASSERT(cons(0, std::make_tuple('1')) == std::make_tuple(0, '1'));
    BOOST_HANA_STATIC_ASSERT(cons(0, std::make_tuple('1', 2.2)) == std::make_tuple(0, '1', 2.2));
    BOOST_HANA_STATIC_ASSERT(cons(0, std::make_tuple('1', 2.2, nullptr)) == std::make_tuple(0, '1', 2.2, nullptr));
}
