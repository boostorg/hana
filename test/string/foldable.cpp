/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string/foldable.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <test/injection.hpp>
#include <test/laws/foldable.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // unpack
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(BOOST_HANA_STRING(""), f),
            f()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(BOOST_HANA_STRING("a"), f),
            f(char_<'a'>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(BOOST_HANA_STRING("ab"), f),
            f(char_<'a'>, char_<'b'>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(BOOST_HANA_STRING("abc"), f),
            f(char_<'a'>, char_<'b'>, char_<'c'>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(BOOST_HANA_STRING("abcd"), f),
            f(char_<'a'>, char_<'b'>, char_<'c'>, char_<'d'>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(BOOST_HANA_STRING("abcde"), f),
            f(char_<'a'>, char_<'b'>, char_<'c'>, char_<'d'>, char_<'e'>)
        ));
    }

    // length
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(length(BOOST_HANA_STRING("")), int_<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(length(BOOST_HANA_STRING("a")), int_<1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(length(BOOST_HANA_STRING("ab")), int_<2>));
        BOOST_HANA_CONSTANT_ASSERT(equal(length(BOOST_HANA_STRING("abc")), int_<3>));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Foldable_laws(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("abc"),
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("abcde")
        ));
    }
}
