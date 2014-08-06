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


BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

// the predicate needs to be compile-time
template <int i>
constexpr auto x = int_<i>;

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto p = detail::minimal::product<>;

    constexpr auto z = x<999>;

    BOOST_HANA_CONSTANT_ASSERT(span(list(), is(z)) == p(list(), list()));

    BOOST_HANA_CONSTANT_ASSERT(span(list(x<0>), is(z)) == p(list(), list(x<0>)));
    BOOST_HANA_CONSTANT_ASSERT(span(list(z), is(z)) == p(list(z), list()));

    BOOST_HANA_CONSTANT_ASSERT(span(list(x<0>, z), is(z)) == p(list(), list(x<0>, z)));
    BOOST_HANA_CONSTANT_ASSERT(span(list(z, x<0>), is(z)) == p(list(z), list(x<0>)));
    BOOST_HANA_CONSTANT_ASSERT(span(list(x<0>, x<1>), is(z)) == p(list(), list(x<0>, x<1>)));

    BOOST_HANA_CONSTANT_ASSERT(span(list(x<0>, x<1>, x<2>), is(z)) == p(list(), list(x<0>, x<1>, x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(span(list(z, x<1>, x<2>), is(z)) == p(list(z), list(x<1>, x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(span(list(x<0>, z, x<2>), is(z)) == p(list(), list(x<0>, z, x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(span(list(z, z, x<2>), is(z)) == p(list(z, z), list(x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(span(list(z, z, z), is(z)) == p(list(z, z, z), list()));
}

int main() {
    test<List::mcd<void>>();
    (void)is;
}
