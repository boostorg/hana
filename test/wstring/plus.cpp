// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_AUTO_STRING(L""), BOOST_HANA_AUTO_STRING(L"")),
        BOOST_HANA_AUTO_STRING(L"")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_AUTO_STRING(L"abcd"), BOOST_HANA_AUTO_STRING(L"")),
        BOOST_HANA_AUTO_STRING(L"abcd")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_AUTO_STRING(L""), BOOST_HANA_AUTO_STRING(L"abcd")),
        BOOST_HANA_AUTO_STRING(L"abcd")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_AUTO_STRING(L"abcd"), BOOST_HANA_AUTO_STRING(L"efg")),
        BOOST_HANA_AUTO_STRING(L"abcdefg")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::plus(BOOST_HANA_AUTO_STRING(L"\xabcd"), BOOST_HANA_AUTO_STRING(L"\xef01")),
        BOOST_HANA_AUTO_STRING(L"\xabcd\xef01")
    ));

    // check operator
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_AUTO_STRING(L"abc") + BOOST_HANA_AUTO_STRING(L"def"),
        BOOST_HANA_AUTO_STRING(L"abcdef")
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_AUTO_STRING(L"\xabcd") + BOOST_HANA_AUTO_STRING(L"\xef01"),
        BOOST_HANA_AUTO_STRING(L"\xabcd\xef01")
    ));
}
