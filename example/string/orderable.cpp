/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/string.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(
        BOOST_HANA_STRING("abc") < BOOST_HANA_STRING("bcd")
    );

    BOOST_HANA_CONSTANT_ASSERT(
        BOOST_HANA_STRING("abcd") > BOOST_HANA_STRING("abc")
    );
    //! [main]
}
