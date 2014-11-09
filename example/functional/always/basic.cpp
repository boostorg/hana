/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_CHECK(always(1)() == 1);
    BOOST_HANA_CONSTEXPR_CHECK(always('2')(1, 2, 3) == '2');
    //! [main]
}
