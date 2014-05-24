/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(equal(type<void>, type<void>));
    BOOST_HANA_STATIC_ASSERT(!not_equal(type<void>, type<void>));

    BOOST_HANA_STATIC_ASSERT(!equal(type<void>, type<int>));
    BOOST_HANA_STATIC_ASSERT(not_equal(type<void>, type<int>));
}
