// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/integral_constant.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::front(BOOST_HANA_BASIC_STRING(C, "a")),
        hana::basic_char_c<C, cast_c<C>('a')>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::front(BOOST_HANA_BASIC_STRING(C, "ab")),
        hana::basic_char_c<C, cast_c<C>('a')>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::front(BOOST_HANA_BASIC_STRING(C, "abc")),
        hana::basic_char_c<C, cast_c<C>('a')>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::front(BOOST_HANA_BASIC_STRING(C, "\xaa" "bc")),
        hana::basic_char_c<C, cast_c<C>('\xaa')>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::front(BOOST_HANA_BASIC_STRING(C, "\xaa\xbb" "c")),
        hana::basic_char_c<C, cast_c<C>('\xaa')>
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
