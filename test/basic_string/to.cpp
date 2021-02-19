// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/basic_string.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;


template <typename C>
void test() {
    static_assert(hana::is_convertible<hana::basic_string_tag<C>, C const*>{}, "");
    static_assert(!hana::is_embedded<hana::basic_string_tag<C>, C const*>{}, "");
    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
           hana::to<C const*>(BOOST_HANA_BASIC_STRING(C, "")),
           static_cast<C const*>(CONVERT_STR(""))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
           hana::to<C const*>(BOOST_HANA_BASIC_STRING(C, "a")),
           static_cast<C const*>(CONVERT_STR("a"))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
           hana::to<C const*>(BOOST_HANA_BASIC_STRING(C, "ab")),
           static_cast<C const*>(CONVERT_STR("ab"))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
           hana::to<C const*>(BOOST_HANA_BASIC_STRING(C, "abc")),
           static_cast<C const*>(CONVERT_STR("abc"))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
           hana::to<C const*>(BOOST_HANA_BASIC_STRING(C, "abcd")),
           static_cast<C const*>(CONVERT_STR("abcd"))
    ) == 0);

    BOOST_HANA_RUNTIME_CHECK(basic_strcmp(
           hana::to<C const*>(BOOST_HANA_BASIC_STRING(C, "ab\x80\xff" "cd")),
           static_cast<C const*>(CONVERT_STR("ab\x80\xff" "cd"))
    ) == 0);


    // make sure we can turn a non-constexpr hana::basic_string<C>
    // into a constexpr C const*
    auto str = BOOST_HANA_BASIC_STRING(C, "abcdef\xc0" "ghi");
    constexpr C const* c_str = hana::to<C const*>(str);
    (void)c_str;
}

int main() {
  test<char>();
  test<wchar_t>();
  test<char16_t>();
  test<char32_t>();
}
