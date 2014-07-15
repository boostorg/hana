/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable/lazy_foldr_mcd.hpp>

#include <boost/hana/detail/minimal/foldable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    constexpr struct { } invalid{};

    BOOST_HANA_STATIC_ASSERT(find(id, foldable()) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int_<1>)) == just(int_<1>));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int_<0>)) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int_<1>, invalid)) == just(int_<1>));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int_<0>, int_<2>)) == just(int_<2>));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int_<0>, int_<0>)) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int_<0>, int_<2>, invalid)) == just(int_<2>));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int_<0>, int_<0>, int_<3>)) == just(int_<3>));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int_<0>, int_<0>, int_<0>)) == nothing);

#if 0
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int{1})) == just(int{1}));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int{0})) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int{1}, long{1})) == just(int{1}));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int{1}, long{0})) == just(int{1}));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int{0}, long{1})) == just(long{1}));
    BOOST_HANA_STATIC_ASSERT(find(id, foldable(int{0}, long{0})) == nothing);
#endif
}

int main() {
    test<Foldable::lazy_foldr_mcd>();
}
