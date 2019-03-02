// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/wstring.hpp>

#include "test_wstring.hpp"
namespace hana = boost::hana;


int main() {
    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_AUTO_STRING(L"").c_str(),
        L""
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_AUTO_STRING(L"a").c_str(),
        L"a"
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_AUTO_STRING(L"ab").c_str(),
        L"ab"
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_AUTO_STRING(L"abc").c_str(),
        L"abc"
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_AUTO_STRING(L"abcd").c_str(),
        L"abcd"
    ) == 0);

    // make sure we can turn a non-constexpr hana::string
    // into a constexpr char const*
    {
        auto str = BOOST_HANA_AUTO_STRING(L"abcdef");
        constexpr wchar_t const* c_str = str.c_str();
        (void)c_str;
    }

    // make sure c_str is actually a static member function
    {
        constexpr wchar_t const* c_str = hana::wstring<'f', 'o', 'o'>::c_str();
        (void)c_str;
    }
}
