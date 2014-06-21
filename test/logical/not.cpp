/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include "minimal_logical.hpp"
using namespace boost::hana;


int main() {
    // not_ is part of the mcd, so there's nothing to test except operators

    using operators::operator!;
    BOOST_HANA_STATIC_ASSERT(!logical<true> == not_(logical<true>));
    BOOST_HANA_STATIC_ASSERT(!logical<false> == not_(logical<false>));
}
