// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::basic_string_tag<C>>(),
        hana::basic_string_c<C>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::basic_string_tag<C>>(
          hana::basic_char_c<C, cast_c<C>('a')>),
        hana::basic_string_c<C, CONVERT_C('a')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::basic_string_tag<C>>(
          hana::basic_char_c<C, cast_c<C>('a')>,
          hana::basic_char_c<C, cast_c<C>('b')>),
        hana::basic_string_c<C, CONVERT_C('a'), CONVERT_C('b')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::basic_string_tag<C>>(
          hana::basic_char_c<C, cast_c<C>('a')>,
          hana::basic_char_c<C, cast_c<C>('b')>,
          hana::basic_char_c<C, cast_c<C>('c')>),
        hana::basic_string_c<C, CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('c')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::basic_string_tag<C>>(
          hana::basic_char_c<C, cast_c<C>('a')>,
          hana::basic_char_c<C, cast_c<C>('b')>,
          hana::basic_char_c<C, cast_c<C>('c')>,
          hana::basic_char_c<C, cast_c<C>('d')>),
        hana::basic_string_c<C, CONVERT_C('a'), CONVERT_C('b'),
                                CONVERT_C('c'), CONVERT_C('d')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::basic_string_tag<C>>(
          hana::basic_char_c<C, cast_c<C>('a')>,
          hana::basic_char_c<C, cast_c<C>('b')>,
          hana::basic_char_c<C, cast_c<C>('c')>,
          hana::basic_char_c<C, cast_c<C>('\xdd')>),
        hana::basic_string_c<C, CONVERT_C('a'), CONVERT_C('b'),
                                CONVERT_C('c'), CONVERT_C('\xdd')>
    ));

    // make sure make_basic_string<C> == make<basic_string_tag<C>>
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make_basic_string<C>(
          hana::basic_char_c<C, cast_c<C>('a')>,
          hana::basic_char_c<C, cast_c<C>('b')>,
          hana::basic_char_c<C, cast_c<C>('c')>),
        hana::make<hana::basic_string_tag<C>>(
          hana::basic_char_c<C, CONVERT_C('a')>,
          hana::basic_char_c<C, CONVERT_C('b')>,
          hana::basic_char_c<C, CONVERT_C('c')>)
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
