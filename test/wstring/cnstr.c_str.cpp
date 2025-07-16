// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/wstring.hpp>

#include <type_traits>
namespace hana = boost::hana;


constexpr wchar_t const empty[] = L"";
constexpr wchar_t const a[] = L"a";
constexpr wchar_t const ab[] = L"ab";
constexpr wchar_t const abc[] = L"abc";
constexpr wchar_t const abcd[] = L"abcd";
constexpr wchar_t const xa[] = L"\xaaaa";
constexpr wchar_t const xabc[] = L"\xaaaa\xbbbb\xcccc";

int main() {
    {
        auto string = hana::to<hana::wstring_tag>(hana::integral_constant<wchar_t const*, empty>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<>));
    }

    {
        auto string = hana::to<hana::wstring_tag>(hana::integral_constant<wchar_t const*, a>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'a'>));
    }

    {
        auto string = hana::to<hana::wstring_tag>(hana::integral_constant<wchar_t const*, ab>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'a', L'b'>));
    }

    {
        auto string = hana::to<hana::wstring_tag>(hana::integral_constant<wchar_t const*, abc>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'a', L'b', L'c'>));
    }

    {
        auto string = hana::to<hana::wstring_tag>(hana::integral_constant<wchar_t const*, abcd>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'a', L'b', L'c', L'd'>));
    }

    {
        auto string = hana::to<hana::wstring_tag>(hana::integral_constant<wchar_t const*, xa>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'\xaaaa'>));
    }

    {
        auto string = hana::to<hana::wstring_tag>(hana::integral_constant<wchar_t const*, xabc>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'\xaaaa', L'\xbbbb', L'\xcccc'>));
    }

    // Make sure it also works with std::integral_constant, for example
    {
        auto string = hana::to<hana::wstring_tag>(std::integral_constant<wchar_t const*, abcd>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'a', L'b', L'c', L'd'>));
    }

    {
        auto string = hana::to<hana::wstring_tag>(std::integral_constant<wchar_t const*, xabc>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'\xaaaa', L'\xbbbb', L'\xcccc'>));
    }

    // Make sure the `to_wstring` shortcut works
    {
        auto string = hana::to_wstring(hana::integral_constant<wchar_t const*, abcd>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'a', L'b', L'c', L'd'>));
    }

    {
        auto string = hana::to_wstring(hana::integral_constant<wchar_t const*, xabc>{});
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::wstring_c<L'\xaaaa', L'\xbbbb', L'\xcccc'>));
    }
}
