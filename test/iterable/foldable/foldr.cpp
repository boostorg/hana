/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable/mcd.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/iterable.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto s) {
    return std::make_tuple(x, s);
};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    constexpr auto iterable = detail::minimal::iterable<mcd>;
    constexpr auto s = x<-1>;

    BOOST_HANA_STATIC_ASSERT(foldr(f, s, iterable()) == s);
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, iterable(x<0>)) == f(x<0>, s));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, iterable(x<0>, x<1>)) == f(x<0>, f(x<1>, s)));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, iterable(x<0>, x<1>, x<2>)) == f(x<0>, f(x<1>, f(x<2>, s))));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, iterable(x<0>, x<1>, x<2>, x<3>)) == f(x<0>, f(x<1>, f(x<2>, f(x<3>, s)))));
}

int main() {
    test<Iterable::mcd>();
}
