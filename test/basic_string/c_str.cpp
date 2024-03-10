// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_BASIC_STRING(C, "").c_str(),
        static_cast<C const*>(CONVERT_STR(""))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_BASIC_STRING(C, "a").c_str(),
        static_cast<C const*>(CONVERT_STR("a"))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_BASIC_STRING(C, "ab").c_str(),
        static_cast<C const*>(CONVERT_STR("ab"))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_BASIC_STRING(C, "abc").c_str(),
        static_cast<C const*>(CONVERT_STR("abc"))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_BASIC_STRING(C, "abcd").c_str(),
        static_cast<C const*>(CONVERT_STR("abcd"))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
        BOOST_HANA_BASIC_STRING(C, "a\xaa" "bc").c_str(),
        static_cast<C const*>(CONVERT_STR("a\xaa" "bc"))
    ) == 0);

    // make sure we can turn a non-constexpr hana::basic_string
    // into a constexpr C const*
    {
        auto str = BOOST_HANA_BASIC_STRING(C, "abc\x88" "def");
        constexpr C const* c_str = str.c_str();
        (void)c_str;
    }

    // make sure c_str is actually a static member function
    {
      constexpr C const* c_str = hana::basic_string<C, CONVERT_C('f'), CONVERT_C('o'), CONVERT_C('o')>::c_str();
        (void)c_str;
    }
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
