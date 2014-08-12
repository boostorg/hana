/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(
        flatten(tuple(tuple(1, 2, 3), tuple(4, 5), tuple(6, 7, 8, 9)))
        ==
        tuple(1, 2, 3, 4, 5, 6, 7, 8, 9)
    );

    BOOST_HANA_CONSTANT_ASSERT(flatten(nothing) == nothing);
    BOOST_HANA_CONSTEXPR_ASSERT(flatten(just(just(1))) == just(1));
    BOOST_HANA_CONSTANT_ASSERT(flatten(just(nothing)) == nothing);
    //! [main]
}
