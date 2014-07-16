/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return std::make_tuple(x);
};

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;

    BOOST_HANA_STATIC_ASSERT(fmap(f, list()) == list());
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(x<1>)) == list(f(x<1>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(x<1>, x<2>)) == list(f(x<1>), f(x<2>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(x<1>, x<2>, x<3>)) == list(f(x<1>), f(x<2>), f(x<3>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(x<1>, x<2>, x<3>, x<4>)) == list(f(x<1>), f(x<2>), f(x<3>), f(x<4>)));
}

int main() {
    test<List::mcd<void>>();
}
