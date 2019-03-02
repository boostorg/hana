// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/greater.hpp>
#include <boost/hana/greater_equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/less_equal.hpp>
#include <boost/hana/not.hpp>
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        BOOST_HANA_BASIC_STRING(C, ""),
        BOOST_HANA_BASIC_STRING(C, "")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_BASIC_STRING(C, ""),
        BOOST_HANA_BASIC_STRING(C, "a")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        BOOST_HANA_BASIC_STRING(C, "a"),
        BOOST_HANA_BASIC_STRING(C, "")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_BASIC_STRING(C, "a"),
        BOOST_HANA_BASIC_STRING(C, "ab")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        BOOST_HANA_BASIC_STRING(C, "ab"),
        BOOST_HANA_BASIC_STRING(C, "ab")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_BASIC_STRING(C, "abc"),
        BOOST_HANA_BASIC_STRING(C, "abcde")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_BASIC_STRING(C, "abcde"),
        BOOST_HANA_BASIC_STRING(C, "abfde")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_BASIC_STRING(C, "abc\xdc"),
        BOOST_HANA_BASIC_STRING(C, "abc\xdd")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_BASIC_STRING(C, "abc"),
        BOOST_HANA_BASIC_STRING(C, "abc\xdd" "e")
    ));

    // check operators
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "abc") < BOOST_HANA_BASIC_STRING(C, "abcd")
    );
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "abc") <= BOOST_HANA_BASIC_STRING(C, "abcd")
    );
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "abc") <= BOOST_HANA_BASIC_STRING(C, "abcd")
    );
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "abc") <= BOOST_HANA_BASIC_STRING(C, "abcd\xdf")
    );
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "abcd") > BOOST_HANA_BASIC_STRING(C, "abc")
    );
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "abc\xd0") >= BOOST_HANA_BASIC_STRING(C, "abc")
    );
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
