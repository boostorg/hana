// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "abcd")[hana::size_c<2>],
        hana::basic_char_c<C, CONVERT_C('c')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "a"), hana::size_c<0>),
        hana::basic_char_c<C, CONVERT_C('a')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "ab"), hana::size_c<0>),
        hana::basic_char_c<C, CONVERT_C('a')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "abc"), hana::size_c<0>),
        hana::basic_char_c<C, CONVERT_C('a')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "\xff" "abc"), hana::size_c<0>),
        hana::basic_char_c<C, CONVERT_C('\xff')>
    ));


    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "ab"), hana::size_c<1>),
        hana::basic_char_c<C, CONVERT_C('b')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "abc"), hana::size_c<1>),
        hana::basic_char_c<C, CONVERT_C('b')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "abcd"), hana::size_c<1>),
        hana::basic_char_c<C, CONVERT_C('b')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "a\xc0" "bcd"), hana::size_c<1>),
        hana::basic_char_c<C, CONVERT_C('\xc0')>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "abc"), hana::size_c<2>),
        hana::basic_char_c<C, CONVERT_C('c')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "abcd"), hana::size_c<2>),
        hana::basic_char_c<C, CONVERT_C('c')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_BASIC_STRING(C, "ab\x80" "cd"), hana::size_c<2>),
        hana::basic_char_c<C, CONVERT_C('\x80')>
    ));
}

int main() {
  test<char>();
}
