/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/minimal/iterable.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

template <int i>
constexpr auto x = detail::number<>(i);

template <typename mcd>
void test() {
    constexpr auto iterable = detail::minimal::iterable<mcd>;
    constexpr auto s = x<-1>;

    // foldl
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl(iterable(), s, f), s));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl(iterable(x<0>), s, f), f(s, x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl(iterable(x<0>, x<1>), s, f), f(f(s, x<0>), x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl(iterable(x<0>, x<1>, x<2>), s, f), f(f(f(s, x<0>), x<1>), x<2>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl(iterable(x<0>, x<1>, x<2>, x<3>), s, f), f(f(f(f(s, x<0>), x<1>), x<2>), x<3>)));
    }

    // foldr1
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr1(iterable(x<0>), f), x<0>));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr1(iterable(x<0>, x<1>), f), f(x<0>, x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr1(iterable(x<0>, x<1>, x<2>), f), f(x<0>, f(x<1>, x<2>))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr1(iterable(x<0>, x<1>, x<2>, x<3>), f), f(x<0>, f(x<1>, f(x<2>, x<3>)))));
    }

    // foldl1
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl1(iterable(x<0>), f), x<0>));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl1(iterable(x<0>, x<1>), f), f(x<0>, x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl1(iterable(x<0>, x<1>, x<2>), f), f(f(x<0>, x<1>), x<2>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl1(iterable(x<0>, x<1>, x<2>, x<3>), f), f(f(f(x<0>, x<1>), x<2>), x<3>)));
    }

    // foldr
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr(iterable(), s, f), s));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr(iterable(x<0>), s, f), f(x<0>, s)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr(iterable(x<0>, x<1>), s, f), f(x<0>, f(x<1>, s))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr(iterable(x<0>, x<1>, x<2>), s, f), f(x<0>, f(x<1>, f(x<2>, s)))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr(iterable(x<0>, x<1>, x<2>, x<3>), s, f), f(x<0>, f(x<1>, f(x<2>, f(x<3>, s))))));
    }
}

int main() {
    test<Iterable::mcd>();
}
