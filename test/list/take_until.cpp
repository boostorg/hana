/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <int ...i>
BOOST_HANA_CONSTEXPR_LAMBDA auto ilist = list_c<int, i...>;

BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % int_<2> != int_<0>;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(take_until(odd, ilist<>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(take_until(odd, ilist<1>) == ilist<>);
    BOOST_HANA_STATIC_ASSERT(take_until(odd, ilist<1, 2>) == ilist<>);

    BOOST_HANA_STATIC_ASSERT(take_until(odd, ilist<0, 1, 2>) == ilist<0>);
    BOOST_HANA_STATIC_ASSERT(take_until(odd, ilist<0, 2, 3>) == ilist<0, 2>);
    BOOST_HANA_STATIC_ASSERT(take_until(odd, ilist<0, 2, 4>) == ilist<0, 2, 4>);
    BOOST_HANA_STATIC_ASSERT(take_until(odd, ilist<0, 2, 1>) == ilist<0, 2>);
}
