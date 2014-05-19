/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/std_array.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <array>
using namespace boost::hana;


template <int ...i>
constexpr auto array = std::array<int, sizeof...(i)>{{i...}};

int main() {
    BOOST_HANA_STATIC_ASSERT(is_empty(array<>));
    BOOST_HANA_STATIC_ASSERT(!is_empty(array<0>));
    BOOST_HANA_STATIC_ASSERT(!is_empty(array<0, 1>));

    BOOST_HANA_STATIC_ASSERT(head(array<0>) == 0);
    BOOST_HANA_STATIC_ASSERT(head(array<0, 1>) == 0);
    BOOST_HANA_STATIC_ASSERT(head(array<0, 1, 2>) == 0);

    BOOST_HANA_STATIC_ASSERT(tail(array<0>) == array<>);
    BOOST_HANA_STATIC_ASSERT(tail(array<0, 1>) == array<1>);
    BOOST_HANA_STATIC_ASSERT(tail(array<0, 1, 2>) == array<1, 2>);
    BOOST_HANA_STATIC_ASSERT(tail(array<0, 1, 2, 3>) == array<1, 2, 3>);
}
