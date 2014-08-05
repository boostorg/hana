/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/minimal/product.hpp>

#include <boost/hana/integral.hpp>
using namespace boost::hana;


constexpr auto prod = detail::minimal::product<>;

// partition requires the predicate to return an Integral boolean, so we need
// the comparison to be purely compile-time.
template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto x = int_<i>;

BOOST_HANA_CONSTEXPR_LAMBDA auto neg = [](auto x) {
    return x < int_<0>;
};

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;

    BOOST_HANA_CONSTANT_ASSERT(partition(list(), neg) == prod(list(), list()));
    BOOST_HANA_CONSTANT_ASSERT(partition(list(x<0>), neg) == prod(list(), list(x<0>)));
    BOOST_HANA_CONSTANT_ASSERT(partition(list(x<0>, x<1>), neg) == prod(list(), list(x<0>, x<1>)));
    BOOST_HANA_CONSTANT_ASSERT(partition(list(x<-1>), neg) == prod(list(x<-1>), list()));
    BOOST_HANA_CONSTANT_ASSERT(partition(list(x<-1>, x<0>, x<2>), neg) == prod(list(x<-1>), list(x<0>, x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(partition(list(x<0>, x<-3>, x<2>, x<-5>, x<6>), neg) == prod(list(x<-3>, x<-5>), list(x<0>, x<2>, x<6>)));
    BOOST_HANA_CONSTANT_ASSERT(partition(list(x<-1>, x<2>, x<-3>, x<0>, x<-3>, x<4>), neg) == prod(list(x<-1>, x<-3>, x<-3>), list(x<2>, x<0>, x<4>)));
}

int main() {
    test<List::mcd<void>>();
    (void)neg;
}
