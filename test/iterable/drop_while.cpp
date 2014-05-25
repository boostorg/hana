/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>

#include "minimal_iterable.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(drop_while(id, iterable()) == iterable());

    BOOST_HANA_STATIC_ASSERT(drop_while(id, iterable(true_)) == iterable());
    BOOST_HANA_STATIC_ASSERT(drop_while(id, iterable(false_)) == iterable(false_));

    BOOST_HANA_STATIC_ASSERT(drop_while(id, iterable(true_, true_)) == iterable());
    BOOST_HANA_STATIC_ASSERT(drop_while(id, iterable(true_, false_)) == iterable(false_));
    BOOST_HANA_STATIC_ASSERT(drop_while(id, iterable(false_, true_)) == iterable(false_, true_));
    BOOST_HANA_STATIC_ASSERT(drop_while(id, iterable(false_, false_)) == iterable(false_, false_));
}
