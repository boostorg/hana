/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/right_folds/variadic_unrolled.hpp>

#include <boost/hana/detail/constexpr.hpp>

#include <cassert>
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

auto test = [](auto foldr) {
    constexpr auto s = x<-1>;
    assert(foldr(f, s) == s);
    assert(foldr(f, s, x<0>) == f(x<0>, s));
    assert(foldr(f, s, x<0>, x<1>) == f(x<0>, f(x<1>, s)));
    assert(foldr(f, s, x<0>, x<1>, x<2>) == f(x<0>, f(x<1>, f(x<2>, s))));
    assert(foldr(f, s, x<0>, x<1>, x<2>, x<3>) == f(x<0>, f(x<1>, f(x<2>, f(x<3>, s)))));
};

int main() {
    test(detail::right_folds::variadic_unrolled);
}
