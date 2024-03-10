// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;


struct invalid { };

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::contains(
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        hana::basic_char_c<C, cast_c<C>('a')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::contains(
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        hana::basic_char_c<C, cast_c<C>('c')>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::contains(
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        hana::basic_char_c<C, cast_c<C>('e')>
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::contains(
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        invalid{}
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::contains(
        BOOST_HANA_BASIC_STRING(C, "ab\xb0" "de"),
        hana::basic_char_c<C, cast_c<C>('\xb0')>
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
