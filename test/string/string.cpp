/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/iterable.hpp>
#include <laws/orderable.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // string and BOOST_HANA_STRING
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto const s1 = BOOST_HANA_STRING("abcd");
        constexpr auto s2 = string<'a', 'b', 'c', 'd'>;

        static_assert(std::is_same<
            decltype(s1), decltype(s2)
        >::value, "");
    }

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
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        BOOST_HANA_CONSTANT_CHECK(equal(
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("abcd")
        ));

        BOOST_HANA_CONSTANT_CHECK(not_(equal(
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("abcde")
        )));

        BOOST_HANA_CONSTANT_CHECK(not_(equal(
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("")
        )));

        BOOST_HANA_CONSTANT_CHECK(not_(equal(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("abcde")
        )));

        BOOST_HANA_CONSTANT_CHECK(equal(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("")
        ));

        // operators
        static_assert(has_operator<String, decltype(equal)>{}, "");
        static_assert(has_operator<String, decltype(not_equal)>{}, "");

        // laws
        test::TestComparable<String>{strings};
    }

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        // less
        BOOST_HANA_CONSTANT_CHECK(not_(less(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("")
        )));

        BOOST_HANA_CONSTANT_CHECK(less(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a")
        ));

        BOOST_HANA_CONSTANT_CHECK(not_(less(
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("")
        )));

        BOOST_HANA_CONSTANT_CHECK(less(
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab")
        ));

        BOOST_HANA_CONSTANT_CHECK(not_(less(
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("ab")
        )));

        BOOST_HANA_CONSTANT_CHECK(less(
            BOOST_HANA_STRING("abc"),
            BOOST_HANA_STRING("abcde")
        ));

        BOOST_HANA_CONSTANT_CHECK(less(
            BOOST_HANA_STRING("abcde"),
            BOOST_HANA_STRING("abfde")
        ));

        // operators
        static_assert(has_operator<String, decltype(less)>{}, "");
        static_assert(has_operator<String, decltype(less_equal)>{}, "");
        static_assert(has_operator<String, decltype(greater)>{}, "");
        static_assert(has_operator<String, decltype(greater_equal)>{}, "");

        // laws
        test::TestOrderable<String>{strings};
    }

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
