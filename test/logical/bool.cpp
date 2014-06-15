/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(eval_if(true,
        [](auto) { return true; },
        [](auto id) { return 1 / id(0); }
    ) == true);
    BOOST_HANA_STATIC_ASSERT(eval_if(false,
        [](auto id) { return 1 / id(0); },
        [](auto) { return false; }
    ) == false);

    BOOST_HANA_STATIC_ASSERT(if_(true, true, false) == true);
    BOOST_HANA_STATIC_ASSERT(if_(false, true, false) == false);
}
