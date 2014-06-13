/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return list(x, -x); };

    BOOST_HANA_STATIC_ASSERT(bind(list(1, 2, 3), f) == list(1, -1, 2, -2, 3, -3));

    BOOST_HANA_STATIC_ASSERT(
        join(list(list(1, 2), list(3, 4), list(list(5, 6)))) == list(1, 2, 3, 4, list(5, 6))
    );

    BOOST_HANA_STATIC_ASSERT(unit<List>('a') == list('a'));
    //! [main]
}
