/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/iterable.hpp>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto strings = make<Tuple>(
        BOOST_HANA_STRING(""),
        BOOST_HANA_STRING("a"),
        BOOST_HANA_STRING("ab"),
        BOOST_HANA_STRING("abc"),
        BOOST_HANA_STRING("abcd"),
        BOOST_HANA_STRING("abcde"),
        BOOST_HANA_STRING("ba"),
        BOOST_HANA_STRING("afcd")
    );

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(BOOST_HANA_STRING("a")),
                char_<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                head(BOOST_HANA_STRING("ab")),
                char_<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                head(BOOST_HANA_STRING("abc")),
                char_<'a'>
            ));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(BOOST_HANA_STRING("a")),
                BOOST_HANA_STRING("")
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(BOOST_HANA_STRING("ab")),
                BOOST_HANA_STRING("b")
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(BOOST_HANA_STRING("abc")),
                BOOST_HANA_STRING("bc")
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(BOOST_HANA_STRING("abcdefghijk")),
                BOOST_HANA_STRING("bcdefghijk")
            ));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(BOOST_HANA_STRING("")));
            BOOST_HANA_CONSTANT_CHECK(is_empty(string<>));

            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("a"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("ab"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("abc"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(string<'a'>)));
        }

        // at
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<0>, BOOST_HANA_STRING("a")),
                char_<'a'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<0>, BOOST_HANA_STRING("ab")),
                char_<'a'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<0>, BOOST_HANA_STRING("abc")),
                char_<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<1>, BOOST_HANA_STRING("ab")),
                char_<'b'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<1>, BOOST_HANA_STRING("abc")),
                char_<'b'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<1>, BOOST_HANA_STRING("abcd")),
                char_<'b'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<2>, BOOST_HANA_STRING("abc")),
                char_<'c'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<2>, BOOST_HANA_STRING("abcd")),
                char_<'c'>
            ));
        }

        // operators
        static_assert(has_operator<String, decltype(at)>{}, "");

        // laws
        test::TestIterable<String>{strings};
    }
}
