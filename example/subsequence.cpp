/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/filter.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/rem.hpp>
#include <boost/hana/subsequence.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


constexpr auto letters = hana::to<hana::tuple_tag>(hana::range_c<char, 'a', 'z'>);
constexpr auto indices = hana::to<hana::tuple_tag>(hana::make_range(hana::int_c<0>, hana::length(letters)));

auto even_indices = hana::filter(indices, [](auto n) {
    return n % hana::uint_c<2> == hana::uint_c<0>;
});

BOOST_HANA_CONSTANT_CHECK(
    hana::subsequence(letters, even_indices) == hana::tuple_c<char,
        'a', 'c', 'e', 'g', 'i', 'k', 'm', 'o', 'q', 's', 'u', 'w', 'y'
    >
);

int main() { }
