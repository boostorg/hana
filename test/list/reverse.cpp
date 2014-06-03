/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int ...i>
BOOST_HANA_CONSTEXPR_LAMBDA auto ilist = list_c<int, i...>;

int main() {
    BOOST_HANA_STATIC_ASSERT(reverse(ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(reverse(ilist<0>) == ilist<0>);
    BOOST_HANA_STATIC_ASSERT(reverse(ilist<0, 1>) == ilist<1, 0>);
    BOOST_HANA_STATIC_ASSERT(reverse(ilist<0, 1, 2>) == ilist<2, 1, 0>);
}
