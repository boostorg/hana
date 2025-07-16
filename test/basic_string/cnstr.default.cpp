// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/basic_string.hpp>

#include "test_basic_string.hpp"
namespace hana = boost::hana;

template <typename C>
void test() {
    using Str = decltype(hana::basic_string_c<C,
                         cast_c<C>('a'),
                         cast_c<C>('b'),
                         cast_c<C>('\xff'),
                         cast_c<C>('d')>);
    Str s{};
    (void)s;
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
