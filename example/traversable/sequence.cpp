/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(
        sequence<Maybe>(list(just(1), just('2'), just(3.3))) == just(list(1, '2', 3.3))
    );

    BOOST_HANA_CONSTANT_ASSERT(
        sequence<Maybe>(list(just(1), nothing, just(3.3))) == nothing
    );

    // This is a generalized Cartesian product.
    BOOST_HANA_CONSTEXPR_ASSERT(
        sequence<List>(list(list(1, 2, 3), list(4), list(5, 6)))
        ==
        list(
            list(1, 4, 5), list(1, 4, 6),
            list(2, 4, 5), list(2, 4, 6),
            list(3, 4, 5), list(3, 4, 6)
        )
    );
    //! [main]
}
