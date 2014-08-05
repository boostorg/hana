/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/iterable.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto iterable = detail::minimal::iterable<mcd>;

    BOOST_HANA_CONSTANT_ASSERT(at(int_<0>, iterable(int_<0>)) == int_<0>);

    BOOST_HANA_CONSTANT_ASSERT(at(int_<0>, iterable(int_<0>, int_<1>)) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(at(int_<1>, iterable(int_<0>, int_<1>)) == int_<1>);

    BOOST_HANA_CONSTANT_ASSERT(at(int_<0>, iterable(int_<0>, int_<1>, int_<2>)) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(at(int_<1>, iterable(int_<0>, int_<1>, int_<2>)) == int_<1>);
    BOOST_HANA_CONSTANT_ASSERT(at(int_<2>, iterable(int_<0>, int_<1>, int_<2>)) == int_<2>);
}

int main() {
    test<Iterable::mcd>();
}
