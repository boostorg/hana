/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(
        concat(list(1, 'a'), list(2, "bb")) == list(1, 'a', 2, "bb")
    );
}
