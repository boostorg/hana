// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/plus.hpp>
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_BASIC_STRING(C, ""),
                   BOOST_HANA_BASIC_STRING(C, "")),
        BOOST_HANA_BASIC_STRING(C, "")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_BASIC_STRING(C, "abcd"),
                   BOOST_HANA_BASIC_STRING(C, "")),
        BOOST_HANA_BASIC_STRING(C, "abcd")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_BASIC_STRING(C, ""),
                   BOOST_HANA_BASIC_STRING(C, "abcd")),
        BOOST_HANA_BASIC_STRING(C, "abcd")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_BASIC_STRING(C, "abcd"),
                   BOOST_HANA_BASIC_STRING(C, "efg")),
        BOOST_HANA_BASIC_STRING(C, "abcdefg")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_BASIC_STRING(C, "ab\xcc"),
                   BOOST_HANA_BASIC_STRING(C, "de\xff")),
        BOOST_HANA_BASIC_STRING(C, "ab\xcc" "de\xff")
    ));

    // check operator
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "abc") + BOOST_HANA_BASIC_STRING(C, "def"),
        BOOST_HANA_BASIC_STRING(C, "abcdef")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "ab\xcc")
            + BOOST_HANA_BASIC_STRING(C, "de\xff"),
        BOOST_HANA_BASIC_STRING(C, "ab\xcc" "de\xff")
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
