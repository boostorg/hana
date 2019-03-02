// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>

#include <type_traits>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    // make sure string_c and BOOST_HANA_STRING give the same result

    {
        auto const s1 = BOOST_HANA_BASIC_STRING(C, "");
        auto const s2 = hana::basic_string_c<C>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
    {
        auto const s1 = BOOST_HANA_BASIC_STRING(C, "a");
        auto const s2 = hana::basic_string_c<C, 'a'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
    {
        auto const s1 = BOOST_HANA_BASIC_STRING(C, "abcd");
        auto const s2 = hana::basic_string_c<C, 'a', 'b', 'c', 'd'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
}

void test_auto() {
    {
        auto const s1 = BOOST_HANA_AUTO_STRING("abc");
        auto const s2 = hana::basic_string_c<char, 'a', 'b', 'c'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
    {
        auto const s1 = BOOST_HANA_AUTO_STRING(L"ab\xabcd");
        auto const s2 = hana::basic_string_c<wchar_t, L'a', L'b', L'\xabcd'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
    {
        auto const s1 = BOOST_HANA_AUTO_STRING(u"ab\xabcd");
        auto const s2 = hana::basic_string_c<char16_t, u'a', u'b', u'\xabcd'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
    {
        auto const s1 = BOOST_HANA_AUTO_STRING(U"ab\xabcdef12");
        auto const s2 = hana::basic_string_c<char32_t, U'a', U'b', U'\xabcdef12'>;
        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();

    test_auto();
}
