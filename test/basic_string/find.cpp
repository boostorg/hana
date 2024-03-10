// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;


struct invalid { };

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_BASIC_STRING(C, "abcd"),
                   hana::basic_char_c<C, cast_c<C>('a')>),
        hana::just(hana::basic_char_c<C, cast_c<C>('a')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_BASIC_STRING(C, "abcd"),
                   hana::basic_char_c<C, cast_c<C>('c')>),
        hana::just(hana::basic_char_c<C, cast_c<C>('c')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_BASIC_STRING(C, "abc\xdcX"),
                   hana::basic_char_c<C, cast_c<C>('X')>),
        hana::just(hana::basic_char_c<C, cast_c<C>('X')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_BASIC_STRING(C, "abc\xff"),
                   hana::basic_char_c<C, cast_c<C>('\xff')>),
        hana::just(hana::basic_char_c<C, cast_c<C>('\xff')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_BASIC_STRING(C, "abcd"), invalid{}),
        hana::nothing
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_BASIC_STRING(C, "abcd\x80"), invalid{}),
        hana::nothing
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_BASIC_STRING(C, "abcd\xe0"),
                   hana::basic_char_c<C, cast_c<C>('e')>),
        hana::nothing
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_BASIC_STRING(C, "abcd"),
                   hana::basic_char_c<C, cast_c<C>('\xaa')>),
        hana::nothing
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
