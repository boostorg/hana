/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/pair.hpp>

#include "minimal.hpp"
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto neg = [](auto x) {
    return x < int_<0>;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<>) == pair(ilist<>, ilist<>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<0>) == pair(ilist<>, ilist<0>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<0, 1>) == pair(ilist<>, ilist<0, 1>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<-1>) == pair(ilist<-1>, ilist<>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<-1, 0, 2>) == pair(ilist<-1>, ilist<0, 2>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<0, -3, 2, -5, 6>) == pair(ilist<-3, -5>, ilist<0, 2, 6>));
    BOOST_HANA_STATIC_ASSERT(partition(neg, ilist<-1, 2, -3, 0, -3, 4>) == pair(ilist<-1, -3, -3>, ilist<2, 0, 4>));
}
