/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


struct undefined { };

int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(
        then(tuple(undefined{}, undefined{}), tuple(1, 2, 3)) == tuple(
            1, 2, 3,
            1, 2, 3
        )
    );
    //! [main]
}
