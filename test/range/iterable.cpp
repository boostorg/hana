/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(is_empty(range(int_<0>, int_<0>)));
    BOOST_HANA_STATIC_ASSERT(!is_empty(range(int_<0>, int_<1>)));
    BOOST_HANA_STATIC_ASSERT(!is_empty(range(int_<0>, int_<2>)));

    BOOST_HANA_STATIC_ASSERT(head(range(int_<0>, int_<1>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(head(range(int_<0>, int_<2>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(head(range(int_<2>, int_<5>)) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(head(range(int_<5>, int_<6>)) == int_<5>);

    BOOST_HANA_STATIC_ASSERT(tail(range(int_<0>, int_<1>)) == range(int_<1>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(tail(range(int_<0>, int_<2>)) == range(int_<1>, int_<2>));
    BOOST_HANA_STATIC_ASSERT(tail(range(int_<0>, int_<3>)) == range(int_<1>, int_<3>));
    BOOST_HANA_STATIC_ASSERT(tail(range(int_<4>, int_<7>)) == range(int_<5>, int_<7>));
}
