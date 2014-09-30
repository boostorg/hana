/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string/constant.hpp>

#include <test/laws/constant.hpp>
using namespace boost::hana;


int main() {
    // value
    {
        auto s = BOOST_HANA_STRING("s");
        static_assert(value(s)[0] == 's' && value(s)[1] == '\0', "");
    }

    // laws
    {
        Constant_laws(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("abc")
        );
    }
}
