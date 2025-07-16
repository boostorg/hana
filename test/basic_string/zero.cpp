// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_string.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/zero.hpp>
namespace hana = boost::hana;


template <typename C>
void test() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
                                hana::zero<hana::basic_string_tag<C>>(),
                                BOOST_HANA_BASIC_STRING(C, "")
    ));
}

int main() {
    test<char>();
    test<wchar_t>();
    test<char16_t>();
    test<char32_t>();
}
