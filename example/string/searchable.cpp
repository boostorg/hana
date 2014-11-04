/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/string.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(char_<'c'> ^in^ BOOST_HANA_STRING("abcde"));
    BOOST_HANA_CONSTANT_ASSERT(!(char_<'z'> ^in^ BOOST_HANA_STRING("abcde")));

    BOOST_HANA_CONSTANT_ASSERT(
        lookup(BOOST_HANA_STRING("abcxefg"), char_<'x'>) == just(char_<'x'>)
    );
    //! [main]
}
