// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/not_equal.hpp> // for operator !=
namespace hana = boost::hana;

template <typename C>
void test() {
    // equal
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        BOOST_HANA_BASIC_STRING(C, "abcd")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "\xffz\xc0"),
        BOOST_HANA_BASIC_STRING(C, "\xffz\xc0")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        BOOST_HANA_BASIC_STRING(C, "abcde")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "abcd"),
        BOOST_HANA_BASIC_STRING(C, "")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_BASIC_STRING(C, ""),
        BOOST_HANA_BASIC_STRING(C, "abcde")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "\xa0"),
        BOOST_HANA_BASIC_STRING(C, "abcde")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_BASIC_STRING(C, "\xa0"),
        BOOST_HANA_BASIC_STRING(C, "")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_BASIC_STRING(C, ""),
        BOOST_HANA_BASIC_STRING(C, "")
    ));

    // check operators
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "abcd")
            ==
        BOOST_HANA_BASIC_STRING(C, "abcd")
    );

    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "abcd")
            !=
        BOOST_HANA_BASIC_STRING(C, "abc")
    );

    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "ab\xc0h")
            ==
        BOOST_HANA_BASIC_STRING(C, "ab\xc0h")
    );

    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_BASIC_STRING(C, "ab\xc0\xd0h")
            !=
        BOOST_HANA_BASIC_STRING(C, "ab\xc0h")
    );

}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
