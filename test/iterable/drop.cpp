/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable.hpp>

#include <boost/hana/detail/minimal/iterable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto iterable = detail::minimal::iterable<mcd>;

    BOOST_HANA_STATIC_ASSERT(drop(int_<0>, iterable()) == iterable());
    BOOST_HANA_STATIC_ASSERT(drop(int_<1>, iterable()) == iterable());
    BOOST_HANA_STATIC_ASSERT(drop(int_<2>, iterable()) == iterable());

    BOOST_HANA_STATIC_ASSERT(drop(int_<0>, iterable(int_<0>)) == iterable(int_<0>));
    BOOST_HANA_STATIC_ASSERT(drop(int_<1>, iterable(int_<0>)) == iterable());
    BOOST_HANA_STATIC_ASSERT(drop(int_<2>, iterable(int_<0>)) == iterable());

    BOOST_HANA_STATIC_ASSERT(drop(int_<0>, iterable(int_<0>, int_<1>)) == iterable(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(drop(int_<1>, iterable(int_<0>, int_<1>)) == iterable(int_<1>));
    BOOST_HANA_STATIC_ASSERT(drop(int_<2>, iterable(int_<0>, int_<1>)) == iterable());
}

int main() {
    test<Iterable::mcd>();
}
