// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/any_of.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::any_of(
                                BOOST_HANA_BASIC_STRING(C, "abcd"),
                                hana::equal.to(hana::basic_char_c<C, CONVERT_C('b')>)
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::any_of(
                                           BOOST_HANA_BASIC_STRING(C, ""),
                                           hana::always(hana::true_c)
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::any_of(
                                           BOOST_HANA_BASIC_STRING(C, "abcd"),
                                           hana::equal.to(hana::basic_char_c<C, CONVERT_C('z')>)
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::any_of(
                                BOOST_HANA_BASIC_STRING(C, "abc\x80" "d"),
                                hana::equal.to(hana::basic_char_c<C, CONVERT_C('\x80')>)
    ));
}

int main() {
  test<char>();
  test<wchar_t>();
  test<char16_t>();
  test<char32_t>();
}
