/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "minimal_iterable.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(all_of(iterable()));
    BOOST_HANA_STATIC_ASSERT(all_of(iterable(true_)));
    BOOST_HANA_STATIC_ASSERT(all_of(iterable(true_, true_)));
    BOOST_HANA_STATIC_ASSERT(!all_of(iterable(true_, true_, false_)));
    BOOST_HANA_STATIC_ASSERT(!all_of(iterable(false_, true_, true_)));

    BOOST_HANA_STATIC_ASSERT(all_of(iterable(true, true)));
    BOOST_HANA_STATIC_ASSERT(!all_of(iterable(true, true, false)));
}
