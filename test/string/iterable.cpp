/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <test/laws/iterable.hpp>
using namespace boost::hana;


int main() {
    // head
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            head(BOOST_HANA_STRING("a")),
            char_<'a'>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            head(BOOST_HANA_STRING("ab")),
            char_<'a'>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            head(BOOST_HANA_STRING("abc")),
            char_<'a'>
        ));
    }

    // tail
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            tail(BOOST_HANA_STRING("a")),
            BOOST_HANA_STRING("")
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            tail(BOOST_HANA_STRING("ab")),
            BOOST_HANA_STRING("b")
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            tail(BOOST_HANA_STRING("abc")),
            BOOST_HANA_STRING("bc")
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            tail(BOOST_HANA_STRING("abcdefghijk")),
            BOOST_HANA_STRING("bcdefghijk")
        ));
    }

    // is_empty
    {
        BOOST_HANA_CONSTANT_ASSERT(is_empty(BOOST_HANA_STRING("")));
        BOOST_HANA_CONSTANT_ASSERT(is_empty(string<>));

        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(BOOST_HANA_STRING("a"))));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(BOOST_HANA_STRING("ab"))));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(BOOST_HANA_STRING("abc"))));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(string<'a'>)));
    }

    // at
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(int_<0>, BOOST_HANA_STRING("a")),
            char_<'a'>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(int_<0>, BOOST_HANA_STRING("ab")),
            char_<'a'>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(int_<0>, BOOST_HANA_STRING("abc")),
            char_<'a'>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(int_<1>, BOOST_HANA_STRING("ab")),
            char_<'b'>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(int_<1>, BOOST_HANA_STRING("abc")),
            char_<'b'>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(int_<1>, BOOST_HANA_STRING("abcd")),
            char_<'b'>
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(int_<2>, BOOST_HANA_STRING("abc")),
            char_<'c'>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            at(int_<2>, BOOST_HANA_STRING("abcd")),
            char_<'c'>
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Iterable_laws(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("abc"),
            BOOST_HANA_STRING("abcd")
        ));
    }
}
