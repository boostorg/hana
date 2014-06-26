/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(snoc(list(), 0) == list(0));
    BOOST_HANA_STATIC_ASSERT(snoc(list('1'), 0) == list('1', 0));
    BOOST_HANA_STATIC_ASSERT(snoc(list('1', 2.2), 0) == list('1', 2.2, 0));
}
