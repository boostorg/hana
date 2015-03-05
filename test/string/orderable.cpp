/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>
#include <laws/orderable.hpp>

#include <type_traits>
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
}
