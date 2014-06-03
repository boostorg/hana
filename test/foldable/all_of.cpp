/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "minimal_foldable.hpp"
using namespace boost::hana;


constexpr struct { } invalid_type{};
constexpr struct {
    constexpr operator bool() const { return *this; }
} invalid_value{};

int main() {
    BOOST_HANA_STATIC_ASSERT(all_of(foldable()));
    BOOST_HANA_STATIC_ASSERT(all_of(foldable(true_)));
    BOOST_HANA_STATIC_ASSERT(all_of(foldable(true_, true_)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(true_, true_, false_)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(false_, true_, true_)));

    BOOST_HANA_STATIC_ASSERT(all_of(foldable(true, true)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(true, true, false)));

    // test short-circuiting
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(false_, invalid_type)));

    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(false_, invalid_value)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(false, invalid_value)));
}
