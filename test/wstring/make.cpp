// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::wstring_tag>(),
        hana::wstring_c<>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::wstring_tag>(hana::wchar_c<L'a'>),
        hana::wstring_c<L'a'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::wstring_tag>(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>),
        hana::wstring_c<L'a', L'b'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::wstring_tag>(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>, hana::wchar_c<L'c'>),
        hana::wstring_c<L'a', L'b', L'c'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::wstring_tag>(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>, hana::wchar_c<L'c'>, hana::wchar_c<L'd'>),
        hana::wstring_c<L'a', L'b', L'c', L'd'>
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make<hana::wstring_tag>(hana::wchar_c<L'\xabcd'>, hana::wchar_c<L'\xef01'>),
        hana::wstring_c<L'\xabcd', L'\xef01'>
    ));

    // make sure make_string == make<string_tag>
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make_wstring(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>, hana::wchar_c<L'c'>),
        hana::make<hana::wstring_tag>(hana::wchar_c<L'a'>, hana::wchar_c<L'b'>, hana::wchar_c<L'c'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::make_wstring(hana::wchar_c<L'\xabcd'>, hana::wchar_c<L'\xef01'>),
        hana::make<hana::wstring_tag>(hana::wchar_c<L'\xabcd'>, hana::wchar_c<L'\xef01'>)
    ));
}
