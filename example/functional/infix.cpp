/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto contains = infix(elem);

    BOOST_HANA_CONSTEXPR_CHECK(
        flip(all)(tuple(1, '2', 3.3) ^ contains,
            tuple(1, '2', 3.3)
        )
    );

    BOOST_HANA_CONSTEXPR_CHECK(
        flip(all)(contains ^ '2',
            tuple(
                tuple(1, '2'),
                tuple('2'),
                tuple('2', 3.3)
            )
        )
    );

    BOOST_HANA_CONSTEXPR_CHECK(tuple(1, '2', 3.3) ^contains^ 3.3);
    //! [main]
}
