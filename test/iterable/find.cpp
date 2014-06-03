/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>

#include "minimal_iterable.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(find(always(true_), iterable()) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(always(true_), iterable(int_<0>)) == just(int_<0>));
    BOOST_HANA_STATIC_ASSERT(find(always(false_), iterable(int_<0>)) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(id, iterable(true_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(true_, false_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(false_, true_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(false_, false_)) == nothing);
}
