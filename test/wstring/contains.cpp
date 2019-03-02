// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


struct invalid { };

int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::contains(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        hana::wchar_c<L'a'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::contains(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        hana::wchar_c<L'c'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::contains(
        BOOST_HANA_AUTO_STRING(L"abcd\xef01"),
        hana::wchar_c<L'a'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::contains(
        BOOST_HANA_AUTO_STRING(L"abcd\xef01"),
        hana::wchar_c<L'\xef01'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::contains(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        hana::wchar_c<L'e'>
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::contains(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        invalid{}
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::contains(
        BOOST_HANA_AUTO_STRING(L"abcd\xefab"),
        hana::wchar_c<L'\xab'>
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::contains(
        BOOST_HANA_AUTO_STRING(L"abcd\xef01"),
        invalid{}
    )));
}
