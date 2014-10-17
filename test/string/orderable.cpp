/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string/orderable.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/laws/orderable.hpp>
using namespace boost::hana;


int main() {
    // less
    {
        BOOST_HANA_CONSTANT_ASSERT(not_(less(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("")
        )));

        BOOST_HANA_CONSTANT_ASSERT(less(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a")
        ));

        BOOST_HANA_CONSTANT_ASSERT(not_(less(
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("")
        )));

        BOOST_HANA_CONSTANT_ASSERT(less(
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab")
        ));

        BOOST_HANA_CONSTANT_ASSERT(not_(less(
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("ab")
        )));

        BOOST_HANA_CONSTANT_ASSERT(less(
            BOOST_HANA_STRING("abc"),
            BOOST_HANA_STRING("abcde")
        ));

        BOOST_HANA_CONSTANT_ASSERT(less(
            BOOST_HANA_STRING("abcde"),
            BOOST_HANA_STRING("abfde")
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Orderable_laws(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("ba"),
            BOOST_HANA_STRING("abc"),
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("afcd")
        ));
    }
}
