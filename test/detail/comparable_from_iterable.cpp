/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/comparable_from_iterable.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>

#include "../iterable/minimal_iterable.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(equal(iterable(), iterable()));
    BOOST_HANA_STATIC_ASSERT(not_equal(iterable(int_<0>), iterable()));
    BOOST_HANA_STATIC_ASSERT(not_equal(iterable(), iterable(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(equal(iterable(int_<0>), iterable(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(not_equal(iterable(int_<0>, int_<1>), iterable(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(equal(iterable(int_<0>, int_<1>), iterable(int_<0>, int_<1>)));

    BOOST_HANA_STATIC_ASSERT(equal(iterable(0), iterable(0)));
    BOOST_HANA_STATIC_ASSERT(not_equal(iterable(0, 1), iterable(0, 2)));
    BOOST_HANA_STATIC_ASSERT(equal(iterable(0, 1, 2), iterable(0, 1, 2)));
}
