/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(nothing == nothing);
    BOOST_HANA_STATIC_ASSERT(nothing != just(int_<0>));
    BOOST_HANA_STATIC_ASSERT(just(int_<0>) != nothing);

    BOOST_HANA_STATIC_ASSERT(just(int_<0>) == just(int_<0>));
    BOOST_HANA_STATIC_ASSERT(just(int_<0>) != just(int_<1>));


    BOOST_HANA_STATIC_ASSERT(just(0) == just(int_<0>));
    BOOST_HANA_STATIC_ASSERT(just(0) != just(int_<1>));
    BOOST_HANA_STATIC_ASSERT(just(int_<0>) == just(0));
    BOOST_HANA_STATIC_ASSERT(just(int_<1>) != just(0));
}
