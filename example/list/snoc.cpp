/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(snoc(list(), 1) == list(1));
    BOOST_HANA_STATIC_ASSERT(snoc(list(1, '2'), 3.3) == list(1, '2', 3.3));
    BOOST_HANA_STATIC_ASSERT(snoc(snoc(snoc(list(), 1), '2'), 3.3) == list(1, '2', 3.3));
    //! [main]
}
