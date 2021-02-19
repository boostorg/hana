// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_AUTO_STRING(L"abcd")[hana::size_c<2>],
        hana::wchar_c<L'c'>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"a"), hana::size_c<0>),
        hana::wchar_c<L'a'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"ab"), hana::size_c<0>),
        hana::wchar_c<L'a'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"abc"), hana::size_c<0>),
        hana::wchar_c<L'a'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"\xabcd"), hana::size_c<0>),
        hana::wchar_c<L'\xabcd'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"\xabcd" L"e"), hana::size_c<0>),
        hana::wchar_c<L'\xabcd'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"\xabcd\xef12"), hana::size_c<0>),
        hana::wchar_c<L'\xabcd'>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"ab"), hana::size_c<1>),
        hana::wchar_c<L'b'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"abc"), hana::size_c<1>),
        hana::wchar_c<L'b'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"abcd"), hana::size_c<1>),
        hana::wchar_c<L'b'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"\xabcd" L"e"), hana::size_c<1>),
        hana::wchar_c<L'e'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"\xabcd\xef12"), hana::size_c<1>),
        hana::wchar_c<L'\xef12'>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"abc"), hana::size_c<2>),
        hana::wchar_c<L'c'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"abcd"), hana::size_c<2>),
        hana::wchar_c<L'c'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"\xabcd" L"ef"), hana::size_c<2>),
        hana::wchar_c<L'f'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::at(BOOST_HANA_AUTO_STRING(L"\xabcd\xef12\x3456"), hana::size_c<2>),
        hana::wchar_c<L'\x3456'>
    ));
}
