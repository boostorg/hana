// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/wstring.hpp>

#include <type_traits>
namespace hana = boost::hana;


int main() {
    // make sure wstring_c and BOOST_HANA_AUTO_STRING give the same result

    {
        auto const s1 = BOOST_HANA_AUTO_STRING(L"");
        auto const s2 = hana::wstring_c<>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
    {
        auto const s1 = BOOST_HANA_AUTO_STRING(L"a");
        auto const s2 = hana::wstring_c<L'a'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
    {
        auto const s1 = BOOST_HANA_AUTO_STRING(L"abcd");
        auto const s2 = hana::wstring_c<L'a', L'b', L'c', L'd'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
    {
        auto const s1 = BOOST_HANA_AUTO_STRING(L"\xabcd\xef01");
        auto const s2 = hana::wstring_c<L'\xabcd', L'\xef01'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
}
