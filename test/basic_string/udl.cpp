// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/config.hpp>

#include <type_traits>
#include "test_basic_string.hpp"
namespace hana = boost::hana;

#ifdef BOOST_HANA_CONFIG_ENABLE_BASIC_STRING_UDL
template <typename C>
struct str1;

template <>
struct str1<char> {
    using namespace hana::literals;
    constexpr static auto value = "abcd"_s;
};

template <>
struct str1<wchar_t> {
    using namespace hana::literals;
    constexpr static auto value = L"abcd"_s;
};

template <>
struct str1<char16_t> {
    using namespace hana::literals;
    constexpr static auto value = u"abcd"_s;
};

template <>
struct str1<char32_t> {
    using namespace hana::literals;
    constexpr static auto value = U"abcd"_s;
};
#endif

template <typename C>
void test() {
    // Check the _s user-defined literal
#ifdef BOOST_HANA_CONFIG_ENABLE_BASIC_STRING_UDL

    constexpr auto s1 = str1<C>::value;
    constexpr auto s2 = hana::string_c<CONVERT_C('a'), CONVERT_C('b'),
                                       CONVERT_C('c'), CONVERT_C('d')>;
    static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
#endif
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
