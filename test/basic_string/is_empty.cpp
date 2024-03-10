// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/not.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::is_empty(BOOST_HANA_BASIC_STRING(C, "")));
    BOOST_HANA_CONSTANT_CHECK(hana::is_empty(hana::basic_string_c<C>));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(BOOST_HANA_BASIC_STRING(C, "a"))));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(BOOST_HANA_BASIC_STRING(C, "ab"))));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(BOOST_HANA_BASIC_STRING(C, "abc"))));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(hana::basic_string_c<C, CONVERT_C('a')>)));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(hana::basic_string_c<C, CONVERT_C('\xff')>)));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
