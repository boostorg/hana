/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(lift<List>('a') == list('a'));

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = _ + _;
    BOOST_HANA_STATIC_ASSERT(
        ap(lift<List>(f), list(1, 2), list(3, 4, 5))
            ==
        list(
            f(1, 3), f(1, 4), f(1, 5),
            f(2, 3), f(2, 4), f(2, 5)
        )
    );
    //! [main]
}
