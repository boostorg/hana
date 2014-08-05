/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTANT_ASSERT(is_empty(std::make_tuple()));
    BOOST_HANA_CONSTANT_ASSERT(!is_empty(std::make_tuple(int_<0>)));
    BOOST_HANA_CONSTANT_ASSERT(!is_empty(std::make_tuple(int_<0>, int_<1>)));

    BOOST_HANA_CONSTANT_ASSERT(head(std::make_tuple(int_<0>)) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(head(std::make_tuple(int_<0>, int_<1>)) == int_<0>);

    BOOST_HANA_CONSTANT_ASSERT(equal(tail(std::make_tuple(int_<0>)), std::make_tuple()));
    BOOST_HANA_CONSTANT_ASSERT(equal(tail(std::make_tuple(int_<0>, int_<1>)), std::make_tuple(int_<1>)));
    BOOST_HANA_CONSTANT_ASSERT(equal(tail(std::make_tuple(int_<0>, int_<1>, int_<2>)), std::make_tuple(int_<1>, int_<2>)));
}
