// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/hash.hpp>
#include <boost/hana/type.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::hash(hana::basic_string_c<C>),
        hana::type_c<hana::basic_string<C>>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
                                hana::hash(hana::basic_string_c<C, CONVERT_C('a')>),
                                hana::type_c<hana::basic_string<C, CONVERT_C('a')>>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
                                hana::hash(hana::basic_string_c<C, CONVERT_C('a'), CONVERT_C('b')>),
                                hana::type_c<hana::basic_string<C, CONVERT_C('a'), CONVERT_C('b')>>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
                                hana::hash(hana::basic_string_c<C, CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('c')>),
                                hana::type_c<hana::basic_string<C, CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('c')>>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::hash(hana::basic_string_c<C,
                   CONVERT_C('\xaa'),
                   CONVERT_C('\xbb'),
                   CONVERT_C('c')>),
        hana::type_c<hana::basic_string<C,
                   CONVERT_C('\xaa'),
                   CONVERT_C('\xbb'),
                   CONVERT_C('c')>>
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
