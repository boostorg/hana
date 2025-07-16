// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/unpack.hpp>

#include <laws/base.hpp>
#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    hana::test::_injection<0> f{};

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_BASIC_STRING(C, ""), f),
        f()
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_BASIC_STRING(C, "a"), f),
        f(hana::basic_char_c<C, CONVERT_C('a')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_BASIC_STRING(C, "ab"), f),
        f(hana::basic_char_c<C, CONVERT_C('a')>,
          hana::basic_char_c<C, CONVERT_C('b')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_BASIC_STRING(C, "abc"), f),
        f(hana::basic_char_c<C, CONVERT_C('a')>,
          hana::basic_char_c<C, CONVERT_C('b')>,
          hana::basic_char_c<C, CONVERT_C('c')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_BASIC_STRING(C, "abcd"), f),
        f(hana::basic_char_c<C, CONVERT_C('a')>,
          hana::basic_char_c<C, CONVERT_C('b')>,
          hana::basic_char_c<C, CONVERT_C('c')>,
          hana::basic_char_c<C, CONVERT_C('d')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_BASIC_STRING(C, "abcde"), f),
        f(hana::basic_char_c<C, CONVERT_C('a')>,
          hana::basic_char_c<C, CONVERT_C('b')>,
          hana::basic_char_c<C, CONVERT_C('c')>,
          hana::basic_char_c<C, CONVERT_C('d')>,
          hana::basic_char_c<C, CONVERT_C('e')>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_BASIC_STRING(C, "ab\xcc" "de\xff"), f),
        f(hana::basic_char_c<C, CONVERT_C('a')>,
          hana::basic_char_c<C, CONVERT_C('b')>,
          hana::basic_char_c<C, CONVERT_C('\xcc')>,
          hana::basic_char_c<C, CONVERT_C('d')>,
          hana::basic_char_c<C, CONVERT_C('e')>,
          hana::basic_char_c<C, CONVERT_C('\xff')>)
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
