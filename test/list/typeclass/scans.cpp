/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::number<>(i);

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    constexpr auto s = x<999>;

    // scanl
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanl(list(), s, f)                 , list(s)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanl(list(x<0>), s, f)             , list(s, f(s, x<0>))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanl(list(x<0>, x<1>), s, f)       , list(s, f(s, x<0>), f(f(s, x<0>), x<1>))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanl(list(x<0>, x<1>, x<2>), s, f) , list(s, f(s, x<0>), f(f(s, x<0>), x<1>), f(f(f(s, x<0>), x<1>), x<2>))));
    }

    // scanr
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanr(list(), s, f)                 , list(                                                               s)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanr(list(x<0>), s, f)             , list(                                                   f(x<0>, s), s)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanr(list(x<0>, x<1>), s, f)       , list(                              f(x<0>, f(x<1>, s)), f(x<1>, s), s)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanr(list(x<0>, x<1>, x<2>), s, f) , list(f(x<0>, f(x<1>, f(x<2>, s))), f(x<1>, f(x<2>, s)), f(x<2>, s), s)));
    }

    // scanl1
    {
        BOOST_HANA_CONSTANT_ASSERT( equal(scanl1(list(), f)                       , list()));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanl1(list(x<0>), f)                   , list(x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanl1(list(x<0>, x<1>), f)             , list(x<0>, f(x<0>, x<1>))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanl1(list(x<0>, x<1>, x<2>), f)       , list(x<0>, f(x<0>, x<1>), f(f(x<0>, x<1>), x<2>))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanl1(list(x<0>, x<1>, x<2>, x<3>), f) , list(x<0>, f(x<0>, x<1>), f(f(x<0>, x<1>), x<2>), f(f(f(x<0>, x<1>), x<2>), x<3>))));
    }

    // scanr1
    {
        BOOST_HANA_CONSTANT_ASSERT( equal(scanr1(list(), f)                 , list(                                           )));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanr1(list(x<0>), f)             , list(                                       x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanr1(list(x<0>, x<1>), f)       , list(                        f(x<0>, x<1>), x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(scanr1(list(x<0>, x<1>, x<2>), f) , list(f(x<0>, f(x<1>, x<2>)), f(x<1>, x<2>), x<2>)));
    }
}

int main() {
    test<List::mcd<void>>();
}
