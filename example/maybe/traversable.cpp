/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto replicate3 = [](auto x) {
        return tuple(x, x, x);
    };

    BOOST_HANA_CONSTEXPR_ASSERT(traverse<Tuple>(replicate3, just(1)) == tuple(just(1), just(1), just(1)));
    BOOST_HANA_CONSTANT_ASSERT(traverse<Tuple>(replicate3, nothing) == tuple(nothing));
    //! [main]
}
