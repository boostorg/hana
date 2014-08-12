/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(
        group_by(equal ^on^ decltype_,
            list(1, 2, 3, 'x', 'y', 4.4, 5.5)
        )
        == list(
            list(1, 2, 3),
            list('x', 'y'),
            list(4.4, 5.5)
        )
    );
    //! [main]
}
