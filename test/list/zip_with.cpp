/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int ...i>
BOOST_HANA_CONSTEXPR_LAMBDA auto ilist = list_c<int, i...>;

BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto x, auto y) { return x + y; };

int main() {
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<>, ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<1>, ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<>, ilist<3>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<1>, ilist<3>) == ilist<1 + 3>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<1, 2>, ilist<3, 4>) == ilist<1 + 3, 2 + 4>);
    BOOST_HANA_STATIC_ASSERT(zip_with(plus, ilist<1, 2, 3, 4>, ilist<5, 6, 7>) == ilist<1 + 5, 2 + 6, 3 + 7>);
}
