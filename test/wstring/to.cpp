// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/wstring.hpp>

#include "test_wstring.hpp"
namespace hana = boost::hana;


static_assert(hana::is_convertible<hana::wstring_tag, wchar_t const*>{}, "");
static_assert(!hana::is_embedded<hana::wstring_tag, wchar_t const*>{}, "");

int main() {
    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        hana::to<wchar_t const*>(BOOST_HANA_AUTO_STRING(L"")),
        L""
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        hana::to<wchar_t const*>(BOOST_HANA_AUTO_STRING(L"a")),
        L"a"
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        hana::to<wchar_t const*>(BOOST_HANA_AUTO_STRING(L"ab")),
        L"ab"
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        hana::to<wchar_t const*>(BOOST_HANA_AUTO_STRING(L"abc")),
        L"abc"
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        hana::to<wchar_t const*>(BOOST_HANA_AUTO_STRING(L"abcd")),
        L"abcd"
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        hana::to<wchar_t const*>(BOOST_HANA_AUTO_STRING(L"\xabcd\xef01")),
        L"\xabcd\xef01"
    ) == 0);

    // make sure we can turn a non-constexpr hana::string
    // into a constexpr char const*
    auto str = BOOST_HANA_AUTO_STRING(L"abcdef\xabcd");
    constexpr wchar_t const* c_str = hana::to<wchar_t const*>(str);
    (void)c_str;
}
