/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>

#include "minimal_foldable.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(count(id, foldable()) == size_t<0>);

    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true_)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(id, foldable(false_)) == size_t<0>);

    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true_, true_)) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true_, false_)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(id, foldable(false_, true_)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(id, foldable(false_, false_)) == size_t<0>);

    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true_, true_, true_)) == size_t<3>);
    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true_, true_, false_)) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true_, false_, true_)) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true_, false_, false_)) == size_t<1>);

    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true, true)) == 2);
    BOOST_HANA_STATIC_ASSERT(count(id, foldable(true, false)) == 1);
}
