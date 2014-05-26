/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <int ...i>
BOOST_HANA_CONSTEXPR_LAMBDA auto ilist = list_c<int, i...>;

int main() {
    BOOST_HANA_STATIC_ASSERT(init(ilist<0>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(init(ilist<0, 1>) == ilist<0>);
    BOOST_HANA_STATIC_ASSERT(init(ilist<0, 1, 2>) == ilist<0, 1>);
    BOOST_HANA_STATIC_ASSERT(init(ilist<0, 1, 2, 3>) == ilist<0, 1, 2>);

    BOOST_HANA_STATIC_ASSERT(init(list(0, "1", '2', 3)) == list(0, "1", '2'));
}
