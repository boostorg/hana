/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto replicate3 = [](auto x) {
        return list(x, x, x);
    };

    BOOST_HANA_STATIC_ASSERT(traverse<List>(replicate3, just(1)) == list(just(1), just(1), just(1)));
    BOOST_HANA_STATIC_ASSERT(traverse<List>(replicate3, nothing) == list(nothing));
    //! [main]
}
