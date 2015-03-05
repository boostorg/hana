/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/foldable.hpp>
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
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING(""), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("a"), f),
                f(char_<'a'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("ab"), f),
                f(char_<'a'>, char_<'b'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abc"), f),
                f(char_<'a'>, char_<'b'>, char_<'c'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abcd"), f),
                f(char_<'a'>, char_<'b'>, char_<'c'>, char_<'d'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abcde"), f),
                f(char_<'a'>, char_<'b'>, char_<'c'>, char_<'d'>, char_<'e'>)
            ));
        }

        // length
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("")), size_t<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("a")), size_t<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("ab")), size_t<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("abc")), size_t<3>
            ));
        }

        // laws
        test::TestFoldable<String>{strings};
    }
}
