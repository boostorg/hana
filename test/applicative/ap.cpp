/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/applicative/mcd.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/applicative.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    constexpr auto a = detail::minimal::applicative<mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
        return std::make_tuple(xs...);
    };

    BOOST_HANA_STATIC_ASSERT(ap(a(f), a(x<0>)) == a(f(x<0>)));
    BOOST_HANA_STATIC_ASSERT(ap(a(f), a(x<0>), a(x<1>)) == a(f(x<0>, x<1>)));
    BOOST_HANA_STATIC_ASSERT(ap(a(f), a(x<0>), a(x<1>), a(x<2>)) == a(f(x<0>, x<1>, x<2>)));
    BOOST_HANA_STATIC_ASSERT(ap(a(f), a(x<0>), a(x<1>), a(x<2>), a(x<3>)) == a(f(x<0>, x<1>, x<2>, x<3>)));
}

int main() {
    test<Applicative::mcd>();
}
