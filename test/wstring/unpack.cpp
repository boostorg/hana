// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/unpack.hpp>
#include <boost/hana/wstring.hpp>

#include <laws/base.hpp>
namespace hana = boost::hana;


int main() {
    hana::test::_injection<0> f{};

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_AUTO_STRING(L""), f),
        f()
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_AUTO_STRING(L"a"), f),
        f(hana::wchar_c<L'a'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_AUTO_STRING(L"ab"), f),
        f(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_AUTO_STRING(L"abc"), f),
        f(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>, hana::wchar_c<L'c'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_AUTO_STRING(L"abcd"), f),
        f(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>,
          hana::wchar_c<L'c'>, hana::wchar_c<L'd'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_AUTO_STRING(L"abcde"), f),
        f(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>,
          hana::wchar_c<L'c'>, hana::wchar_c<L'd'>, hana::wchar_c<L'e'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::unpack(BOOST_HANA_AUTO_STRING(L"\xabcd\xef01"), f),
        f(hana::wchar_c<L'\xabcd'>, hana::wchar_c<L'\xef01'>)
    ));
}
