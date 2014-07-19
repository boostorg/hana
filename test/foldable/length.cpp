/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable/mcd.hpp>

#include <boost/hana/detail/minimal/foldable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    BOOST_HANA_STATIC_ASSERT(length(foldable()) == size_t<0>);
    BOOST_HANA_STATIC_ASSERT(length(foldable(1)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(length(foldable(1, '2')) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(length(foldable(1, '2', 3.3)) == size_t<3>);
    BOOST_HANA_STATIC_ASSERT(length(foldable(1, '2', 3.3, 4.4f)) == size_t<4>);
    BOOST_HANA_STATIC_ASSERT(length(foldable(1, '2', 3.3, 4.4f, nullptr)) == size_t<5>);

    BOOST_HANA_STATIC_ASSERT(length(foldable()) == size_t<0>);
    BOOST_HANA_STATIC_ASSERT(length(foldable(int_<0>)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(length(foldable(int_<0>, int_<1>)) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(length(foldable(int_<0>, int_<1>, int_<2>)) == size_t<3>);
}

int main() {
    test<Foldable::mcd>();
}
