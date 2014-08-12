/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(
        zip(list(1, 'a'), list(2, 3.3))
        ==
        list(list(1, 2), list('a', 3.3))
    );

    BOOST_HANA_CONSTEXPR_ASSERT(
        zip(list(1, 'a'), list(2, 3.3), list(3, 'c', "ignored"))
        ==
        list(list(1, 2, 3), list('a', 3.3, 'c'))
    );
    //! [main]
}
