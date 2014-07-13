/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/foldable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
    return std::make_tuple(xs...);
};

template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    BOOST_HANA_STATIC_ASSERT(unpack(f, foldable()) == f());
    BOOST_HANA_STATIC_ASSERT(unpack(f, foldable(1)) == f(1));
    BOOST_HANA_STATIC_ASSERT(unpack(f, foldable(1, '2')) == f(1, '2'));
    BOOST_HANA_STATIC_ASSERT(unpack(f, foldable(1, '2', 3.3)) == f(1, '2', 3.3));
    BOOST_HANA_STATIC_ASSERT(unpack(f, foldable(1, '2', 3.3, nullptr)) == f(1, '2', 3.3, nullptr));

    BOOST_HANA_STATIC_ASSERT(unpack(f, foldable(int_<0>)) == f(int_<0>));
    BOOST_HANA_STATIC_ASSERT(unpack(f, foldable(int_<0>, int_<1>)) == f(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(unpack(f, foldable(int_<0>, int_<1>, int_<2>)) == f(int_<0>, int_<1>, int_<2>));
}

int main() {
    test<Foldable::lazy_foldr_mcd>();
}
