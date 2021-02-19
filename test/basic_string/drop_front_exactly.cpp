// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/drop_front_exactly.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
namespace hana = boost::hana;


template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "a")),
        BOOST_HANA_BASIC_STRING(C, "")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "ab")),
        BOOST_HANA_BASIC_STRING(C, "b")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "abc")),
        BOOST_HANA_BASIC_STRING(C, "bc")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "abcdefghijk")),
        BOOST_HANA_BASIC_STRING(C, "bcdefghijk")
    ));


    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "abc"), hana::size_c<2>),
        BOOST_HANA_BASIC_STRING(C, "c")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "abcdefghijk"), hana::size_c<3>),
        BOOST_HANA_BASIC_STRING(C, "defghijk")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "\xa0" "bc")),
        BOOST_HANA_BASIC_STRING(C, "bc")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "a\xbb" "c")),
        BOOST_HANA_BASIC_STRING(C, "\xbb" "c")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "\xa0\xb0" "c"), hana::size_c<2>),
        BOOST_HANA_BASIC_STRING(C, "c")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_BASIC_STRING(C, "a\xb0" "\xc4"), hana::size_c<2>),
        BOOST_HANA_BASIC_STRING(C, "\xc4")
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
