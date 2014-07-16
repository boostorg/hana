/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <boost/hana/integral.hpp>
using namespace boost::hana;


// sort requires the predicate to return an Integral boolean, so we need
// the comparison to be purely compile-time.
template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto x = int_<i>;

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;

    BOOST_HANA_STATIC_ASSERT(sort(list()) == list());
    BOOST_HANA_STATIC_ASSERT(sort(list(x<0>)) == list(x<0>));
    BOOST_HANA_STATIC_ASSERT(sort(list(x<0>, x<1>)) == list(x<0>, x<1>));
    BOOST_HANA_STATIC_ASSERT(sort(list(x<1>, x<0>)) == list(x<0>, x<1>));
    BOOST_HANA_STATIC_ASSERT(sort(list(x<1>, x<0>, x<4>, x<2>)) == list(x<0>, x<1>, x<2>, x<4>));
    BOOST_HANA_STATIC_ASSERT(sort(list(x<1>, x<0>, x<-4>, x<2>)) == list(x<-4>, x<0>, x<1>, x<2>));
}

int main() {
    test<List::mcd<void>>();
}
