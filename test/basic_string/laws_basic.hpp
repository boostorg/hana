// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/basic_string.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/hashable.hpp>
#include <laws/iterable.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <laws/searchable.hpp>
#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void testComparable() {
    auto strings = hana::make_tuple(
        BOOST_HANA_BASIC_STRING(C, ""),
        BOOST_HANA_BASIC_STRING(C, "a"),
        BOOST_HANA_BASIC_STRING(C, "ab"),
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        BOOST_HANA_BASIC_STRING(C, "abcde"),
        BOOST_HANA_BASIC_STRING(C, "b\xaa" "a"),
        BOOST_HANA_BASIC_STRING(C, "\x80\xaa" "c")
    );

    hana::test::TestComparable<hana::basic_string_tag<C>>{strings};
    hana::test::TestHashable<hana::basic_string_tag<C>>{strings};
}

template <typename C>
void testMonoid() {
    auto strings = hana::make_tuple(
        BOOST_HANA_BASIC_STRING(C, ""),
        BOOST_HANA_BASIC_STRING(C, "a"),
        BOOST_HANA_BASIC_STRING(C, "ab"),
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        BOOST_HANA_BASIC_STRING(C, "abcde"),
        BOOST_HANA_BASIC_STRING(C, "b\xaa" "a"),
        BOOST_HANA_BASIC_STRING(C, "\x80\xaa" "c")
    );

    hana::test::TestMonoid<hana::basic_string_tag<C>>{strings};
}

template <typename C>
void testFoldable() {
    auto strings = hana::make_tuple(
        BOOST_HANA_BASIC_STRING(C, ""),
        BOOST_HANA_BASIC_STRING(C, "a"),
        BOOST_HANA_BASIC_STRING(C, "ab"),
        BOOST_HANA_BASIC_STRING(C, "abcde"),
        BOOST_HANA_BASIC_STRING(C, "ba"),
        BOOST_HANA_BASIC_STRING(C, "afcd"),
        BOOST_HANA_BASIC_STRING(C, "b\xaa" "a"),
        BOOST_HANA_BASIC_STRING(C, "\x80\xaa" "c")
    );

    hana::test::TestFoldable<hana::basic_string_tag<C>>{strings};
    hana::test::TestIterable<hana::basic_string_tag<C>>{strings};
}

template <typename C>
void testOrderable() {
    auto strings = hana::make_tuple(
        BOOST_HANA_BASIC_STRING(C, ""),
        BOOST_HANA_BASIC_STRING(C, "a"),
        BOOST_HANA_BASIC_STRING(C, "ab"),
        BOOST_HANA_BASIC_STRING(C, "abc"),
        BOOST_HANA_BASIC_STRING(C, "ba"),
        BOOST_HANA_BASIC_STRING(C, "b\xaa" "a"),
        BOOST_HANA_BASIC_STRING(C, "\x80\xaa" "c")
    );

    hana::test::TestOrderable<hana::basic_string_tag<C>>{strings};
}

template <typename C>
void testSearchable() {
     auto keys = hana::tuple_c<C, cast_c<C>('\x80'), cast_c<C>('\xaa')>;
     auto strings = hana::make_tuple(
         BOOST_HANA_BASIC_STRING(C, ""),
         BOOST_HANA_BASIC_STRING(C, "a"),
         BOOST_HANA_BASIC_STRING(C, "abcd"),
         BOOST_HANA_BASIC_STRING(C, "ba"),
         BOOST_HANA_BASIC_STRING(C, "b\xaa" "a"),
         BOOST_HANA_BASIC_STRING(C, "\x80\xaa" "c")
     );

     hana::test::TestSearchable<hana::basic_string_tag<C>>{strings, keys};
}
