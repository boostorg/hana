/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_CHECK(if_(true, 1, 2) == 1);
    BOOST_HANA_CONSTEXPR_CHECK(if_(false, 1, 2) == 2);

    BOOST_HANA_CONSTEXPR_CHECK(
        if_(true_,
            tuple('t', 'r', 'u', 'e'),
            tuple('f', 'a', 'l', 's', 'e')
        )
        ==
        tuple('t', 'r', 'u', 'e')
    );
    //! [main]
}
