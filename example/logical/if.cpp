/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(if_(true, 1, 2) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(if_(false, 1, 2) == 2);

    BOOST_HANA_CONSTEXPR_ASSERT(
        if_(true_,
            list('t', 'r', 'u', 'e'),
            list('f', 'a', 'l', 's', 'e')
        )
        ==
        list('t', 'r', 'u', 'e')
    );
    //! [main]
}
