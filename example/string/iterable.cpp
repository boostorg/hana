/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/string.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_CHECK(!is_empty(BOOST_HANA_STRING("abcd")));
    BOOST_HANA_CONSTANT_CHECK(is_empty(BOOST_HANA_STRING("")));

    BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("abcd")[int_<2>] == char_<'c'>);

    auto is_vowel = [](auto c) {
        return c ^in^ BOOST_HANA_STRING("aeiouy");
    };
    BOOST_HANA_CONSTANT_CHECK(
        drop_while(BOOST_HANA_STRING("aioubcd"), is_vowel) == BOOST_HANA_STRING("bcd")
    );
    //! [main]
}
