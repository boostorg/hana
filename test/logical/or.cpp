/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include "minimal_logical.hpp"
using namespace boost::hana;


constexpr struct { } invalid{};

int main() {
    BOOST_HANA_STATIC_ASSERT(!or_());

    BOOST_HANA_STATIC_ASSERT(or_(logical<true>));
    BOOST_HANA_STATIC_ASSERT(!or_(logical<false>));

    BOOST_HANA_STATIC_ASSERT(!or_(logical<false>, logical<false>));
    BOOST_HANA_STATIC_ASSERT(or_(logical<false>, logical<true>));
    // BOOST_HANA_STATIC_ASSERT(or_(logical<true>, invalid));

    BOOST_HANA_STATIC_ASSERT(!or_(logical<false>, logical<false>, logical<false>));
    BOOST_HANA_STATIC_ASSERT(or_(logical<false>, logical<false>, logical<true>));
    // BOOST_HANA_STATIC_ASSERT(or_(logical<false>, logical<true>, invalid));
    // BOOST_HANA_STATIC_ASSERT(or_(logical<true>, invalid, invalid));
}
