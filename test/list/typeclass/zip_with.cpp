/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

BOOST_HANA_CONSTEXPR_LAMBDA auto f1 = [](auto x) {
    return std::make_tuple(x);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto f2 = [](auto x, auto y) {
    return std::make_tuple(x, y);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto fn = [](auto ...xs) {
    return std::make_tuple(xs...);
};

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;

    BOOST_HANA_CONSTANT_ASSERT(zip_with(f1, list()) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(f1, list(x<0>)) == list(f1(x<0>)));
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(f1, list(x<0>, x<1>)) == list(f1(x<0>), f1(x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(f1, list(x<0>, x<1>, x<2>)) == list(f1(x<0>), f1(x<1>), f1(x<2>)));
    BOOST_HANA_CONSTANT_ASSERT(zip_with(f1, list(), list()) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(f1, list(), list(x<1>)) == list());

    BOOST_HANA_CONSTANT_ASSERT(zip_with(f2, list()) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(f2, list(), list()) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(f2, list(x<0>), list()) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(f2, list(), list(x<0>)) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(f2, list(x<0>), list(x<1>)) == list(f2(x<0>, x<1>)));
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(f2, list(x<0>, x<1>), list(x<2>, x<3>)) == list(f2(x<0>, x<2>), f2(x<1>, x<3>)));
    BOOST_HANA_CONSTEXPR_ASSERT(zip_with(f2, list(x<1>, x<2>, x<3>, x<4>), list(x<5>, x<6>, x<7>)) == list(f2(x<1>, x<5>), f2(x<2>, x<6>), f2(x<3>, x<7>)));
    BOOST_HANA_CONSTANT_ASSERT(zip_with(f2, list(), list(), list()) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(f2, list(), list(), list(x<1>)) == list());
    BOOST_HANA_CONSTANT_ASSERT(zip_with(f2, list(), list(x<1>), list(x<2>)) == list());

    BOOST_HANA_CONSTEXPR_ASSERT(
        zip_with(fn,
            list(x<11>, x<12>, x<13>, x<14>),
            list(x<21>, x<22>, x<23>),
            list(x<31>, x<32>, x<33>, x<34>),
            list(x<41>, x<42>, x<43>, x<44>, x<45>)
        )
        ==
        list(
            fn(x<11>, x<21>, x<31>, x<41>),
            fn(x<12>, x<22>, x<32>, x<42>),
            fn(x<13>, x<23>, x<33>, x<43>)
        )
    );
}

int main() {
    test<List::mcd<void>>();
}
