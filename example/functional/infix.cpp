/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto contains = infix(elem);

    BOOST_HANA_CONSTEXPR_CHECK(
        flip(all_of)(make<Tuple>(1, '2', 3.3) ^ contains,
            make<Tuple>(1, '2', 3.3)
        )
    );

    BOOST_HANA_CONSTEXPR_CHECK(
        flip(all_of)(contains ^ '2',
            make<Tuple>(
                make<Tuple>(1, '2'),
                make<Tuple>('2'),
                make<Tuple>('2', 3.3)
            )
        )
    );

    BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, '2', 3.3) ^contains^ 3.3);
    //! [main]
}
