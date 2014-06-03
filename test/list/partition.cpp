/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto neg = _ < int_<0>;

template <int ...i>
BOOST_HANA_CONSTEXPR_LAMBDA auto ilist = list_c<int, i...>;

int main() {
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<>) == list(ilist<>, ilist<>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<0>) == list(ilist<>, ilist<0>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<0, 1>) == list(ilist<>, ilist<0, 1>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<-1>) == list(ilist<-1>, ilist<>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<-1, 0, 2>) == list(ilist<-1>, ilist<0, 2>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<0, -3, 2, -5, 6>) == list(ilist<-3, -5>, ilist<0, 2, 6>));
}
