/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable/lazy_foldr_mcd.hpp>

#include <boost/hana/detail/minimal/foldable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    // compile-time
    BOOST_HANA_STATIC_ASSERT(!elem(int_<0>, foldable()));

    BOOST_HANA_STATIC_ASSERT( elem(int_<0>, foldable(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(!elem(int_<0>, foldable(int_<1>)));

    BOOST_HANA_STATIC_ASSERT( elem(int_<0>, foldable(int_<0>, int_<1>)));
    BOOST_HANA_STATIC_ASSERT(!elem(int_<0>, foldable(int_<1>, int_<2>)));
    BOOST_HANA_STATIC_ASSERT( elem(int_<0>, foldable(int_<1>, int_<0>)));

    BOOST_HANA_STATIC_ASSERT( elem(int_<0>, foldable(int_<1>, int_<0>, int_<3>)));
    BOOST_HANA_STATIC_ASSERT(!elem(int_<0>, foldable(int_<1>, int_<2>, int_<3>)));
    BOOST_HANA_STATIC_ASSERT( elem(int_<0>, foldable(int_<1>, int_<2>, int_<0>)));
    BOOST_HANA_STATIC_ASSERT( elem(int_<0>, foldable(int_<0>, int_<2>, int_<0>)));


    // constexpr/runtime
    BOOST_HANA_STATIC_ASSERT( elem(0, foldable(0)));
    BOOST_HANA_STATIC_ASSERT(!elem(0, foldable(1)));

    BOOST_HANA_STATIC_ASSERT( elem(0, foldable(0, 1)));
    BOOST_HANA_STATIC_ASSERT(!elem(0, foldable(1, 2)));
    BOOST_HANA_STATIC_ASSERT( elem(0, foldable(1, 0)));

    BOOST_HANA_STATIC_ASSERT( elem(0, foldable(1, 0, 3)));
    BOOST_HANA_STATIC_ASSERT(!elem(0, foldable(1, 2, 3)));
    BOOST_HANA_STATIC_ASSERT( elem(0, foldable(1, 2, 0)));
    BOOST_HANA_STATIC_ASSERT( elem(0, foldable(0, 2, 0)));
}

int main() {
    test<Foldable::lazy_foldr_mcd>();
}
