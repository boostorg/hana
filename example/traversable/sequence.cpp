/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(
        sequence<Maybe>(tuple(just(1), just('2'), just(3.3))) == just(tuple(1, '2', 3.3))
    );

    BOOST_HANA_CONSTANT_ASSERT(
        sequence<Maybe>(tuple(just(1), nothing, just(3.3))) == nothing
    );

    // This is a generalized Cartesian product.
    BOOST_HANA_CONSTEXPR_ASSERT(
        sequence<Tuple>(tuple(tuple(1, 2, 3), tuple(4), tuple(5, 6)))
        ==
        tuple(
            tuple(1, 4, 5), tuple(1, 4, 6),
            tuple(2, 4, 5), tuple(2, 4, 6),
            tuple(3, 4, 5), tuple(3, 4, 6)
        )
    );
    //! [main]
}
