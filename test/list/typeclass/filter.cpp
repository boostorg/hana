/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/list.hpp>

#include <boost/hana/integral.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto out = [](auto x) {
    return [=](auto y) { return not_equal(x, y); };
};

// filter requires the predicate to return an Integral boolean, so we need
// the comparison to be purely compile-time.
template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto x = int_<i>;

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto z = x<999>;

    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list()) == list());

    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(z)) == list());
    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(x<1>)) == list(x<1>));

    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(x<1>, x<2>)) == list(x<1>, x<2>));
    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(z, x<2>)) == list(x<2>));
    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(x<1>, z)) == list(x<1>));

    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(z, x<2>, x<3>)) == list(x<2>, x<3>));
    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(x<1>, z, x<3>)) == list(x<1>, x<3>));
    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(x<1>, x<2>, z)) == list(x<1>, x<2>));
    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(x<1>, z, z)) == list(x<1>));
    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(z, x<2>, z)) == list(x<2>));
    BOOST_HANA_CONSTANT_ASSERT(filter(out(z), list(z, z, x<3>)) == list(x<3>));
}

int main() {
    test<List::mcd<void>>();
    (void)out;
}
