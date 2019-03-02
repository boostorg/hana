// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find_if.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find_if(BOOST_HANA_BASIC_STRING(C, ""),
                      hana::always(hana::true_c)),
        hana::nothing
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find_if(BOOST_HANA_BASIC_STRING(C, "abcd"),
                      hana::equal.to(hana::basic_char_c<C, cast_c<C>('a')>)),
        hana::just(hana::basic_char_c<C, cast_c<C>('a')>)
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find_if(BOOST_HANA_BASIC_STRING(C, "abcd"),
                      hana::equal.to(hana::basic_char_c<C, cast_c<C>('c')>)),
        hana::just(hana::basic_char_c<C, cast_c<C>('c')>)
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find_if(BOOST_HANA_BASIC_STRING(C, "abcd"),
                      hana::equal.to(hana::basic_char_c<C, cast_c<C>('d')>)),
        hana::just(hana::basic_char_c<C, cast_c<C>('d')>)
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find_if(BOOST_HANA_BASIC_STRING(C, "\xa0" "bc"),
                      hana::equal.to(hana::basic_char_c<C, cast_c<C>('c')>)),
        hana::just(hana::basic_char_c<C, cast_c<C>('c')>)
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find_if(BOOST_HANA_BASIC_STRING(C, "ab\xcc"),
                      hana::equal.to(hana::basic_char_c<C, cast_c<C>('\xcc')>)),
        hana::just(hana::basic_char_c<C, cast_c<C>('\xcc')>)
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
