/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type_list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>

#include <tuple>
using namespace boost::hana;


struct x0; struct x1; struct x2; struct x3;

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto y) {
    return std::make_tuple(x, y);
};

template <typename ...> struct G;
constexpr auto g = template_<G>;

int main() {
    constexpr auto s = type<x0>;
    BOOST_HANA_STATIC_ASSERT(foldl(f, s, type_list<>) == s);
    BOOST_HANA_STATIC_ASSERT(foldl(f, s, type_list<x1>) == f(s, type<x1>));
    BOOST_HANA_STATIC_ASSERT(foldl(f, s, type_list<x1, x2>) == f(f(s, type<x1>), type<x2>));
    BOOST_HANA_STATIC_ASSERT(foldl(f, s, type_list<x1, x2, x3>) == f(f(f(s, type<x1>), type<x2>), type<x3>));

    BOOST_HANA_STATIC_ASSERT(foldl(g, s, type_list<>) == s);
    BOOST_HANA_STATIC_ASSERT(foldl(g, s, type_list<x1>) == g(s, type<x1>));
    BOOST_HANA_STATIC_ASSERT(foldl(g, s, type_list<x1, x2>) == g(g(s, type<x1>), type<x2>));
    BOOST_HANA_STATIC_ASSERT(foldl(g, s, type_list<x1, x2, x3>) == g(g(g(s, type<x1>), type<x2>), type<x3>));
}
