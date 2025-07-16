// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>

#include <type_traits>
#include "test_basic_string.hpp"

namespace hana = boost::hana;

template <typename C>
constexpr C const empty[] = { CONVERT_C('\0') };
template <typename C>
constexpr C const a[] = { CONVERT_C('a'), CONVERT_C('\0') };
template <typename C>
constexpr C const ab[] = { CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('\0') };
template <typename C>
constexpr C const abc[] = { CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('c'), CONVERT_C('\0') };
template <typename C>
constexpr C const abcd[] = { CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('c'), CONVERT_C('d'), CONVERT_C('\0') };
template <typename C>
constexpr C const abxc[] = { CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('\xa0'), CONVERT_C('c'), CONVERT_C('\0') };

template <typename C>
void test() {
    {
        auto string = hana::to<hana::basic_string_tag<C>>(
            hana::integral_constant<C const*, empty<C>>{}
        );
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::basic_string_c<C>));
    }

    {
        auto string = hana::to<hana::basic_string_tag<C>>(
            hana::integral_constant<C const*, a<C>>{}
        );
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::basic_string_c<C,
                                              cast_c<C>('a')>));
    }

    {
        auto string = hana::to<hana::basic_string_tag<C>>(
            hana::integral_constant<C const*, ab<C>>{}
        );
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::basic_string_c<C,
                                              cast_c<C>('a'),
                                              cast_c<C>('b')>));
    }

    {
        auto string = hana::to<hana::basic_string_tag<C>>(
            hana::integral_constant<C const*, abc<C>>{}
        );
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::basic_string_c<C,
                                              cast_c<C>('a'),
                                              cast_c<C>('b'),
                                              cast_c<C>('c')
                                              >));
    }

    {
        auto string = hana::to<hana::basic_string_tag<C>>(
            hana::integral_constant<C const*, abcd<C>>{}
        );
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::basic_string_c<C,
                                              cast_c<C>('a'),
                                              cast_c<C>('b'),
                                              cast_c<C>('c'),
                                              cast_c<C>('d')
                                              >));
    }

    {
        auto string = hana::to<hana::basic_string_tag<C>>(
            hana::integral_constant<C const*, abxc<C>>{}
        );
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::basic_string_c<C,
                                              cast_c<C>('a'),
                                              cast_c<C>('b'),
                                              cast_c<C>('\xa0'),
                                              cast_c<C>('c')
                                              >));
    }

    // Make sure it also works with std::integral_constant, for example
    {
        auto string = hana::to<hana::basic_string_tag<C>>(
            std::integral_constant<C const*, abcd<C>>{}
        );
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::basic_string_c<C, CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('c'), CONVERT_C('d')>));
    }
    // Make sure the `to_string` shortcut works
    {
        auto string = hana::to_basic_string<C>(
            hana::integral_constant<C const*, abcd<C>>{}
        );
        BOOST_HANA_CONSTANT_CHECK(hana::equal(string, hana::basic_string_c<C, CONVERT_C('a'), CONVERT_C('b'), CONVERT_C('c'), CONVERT_C('d')>));
    }
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
