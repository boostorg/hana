/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto s) {
    return std::make_tuple(x, s);
};

template <int i>
struct X {
    template <int j>
    constexpr bool operator==(X<j>) const { return i == j; }
};

template <int i>
constexpr X<i> x{};

int main() {
    constexpr auto s = x<-1>;
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, list()) == s);
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, list(x<0>)) == f(x<0>, s));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, list(x<0>, x<1>)) == f(x<0>, f(x<1>, s)));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, list(x<0>, x<1>, x<2>)) == f(x<0>, f(x<1>, f(x<2>, s))));
}
