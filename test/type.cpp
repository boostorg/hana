/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <bool b = false>
struct invalid { static_assert(b, "invalid must not be instantiated"); };

int main() {
    BOOST_HANA_STATIC_ASSERT(type<void> == type<void>);
    BOOST_HANA_STATIC_ASSERT(type<void> != type<int>);

#if 0
    BOOST_HANA_STATIC_ASSERT(type<invalid<>> == type<invalid<>>);
    BOOST_HANA_STATIC_ASSERT(type<invalid<>> != type<int>);
#endif
}
