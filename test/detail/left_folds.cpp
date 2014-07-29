/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/left_folds/variadic.hpp>
#include <boost/hana/detail/left_folds/variadic_unrolled.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


template <typename ...> struct F;
constexpr auto f = template_<F>;
BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto s, auto x) {
    return type<F<typename decltype(s)::type, typename decltype(x)::type>>;
};

struct x0; struct x1; struct x2; struct x3;

template <typename Foldl, typename F>
constexpr void test_foldl(Foldl foldl, F f) {
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, int_<1>) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, int_<1>, int_<2>) == list(list(int_<0>, int_<1>), int_<2>));
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, int_<1>, int_<2>, int_<3>) == list(list(list(int_<0>, int_<1>), int_<2>), int_<3>));

    BOOST_HANA_STATIC_ASSERT(foldl(f, type<x0>) == type<x0>);
    BOOST_HANA_STATIC_ASSERT(foldl(f, type<x0>, type<x1>) == f(type<x0>, type<x1>));
    BOOST_HANA_STATIC_ASSERT(foldl(f, type<x0>, type<x1>, type<x2>) == f(f(type<x0>, type<x1>), type<x2>));
    BOOST_HANA_STATIC_ASSERT(foldl(f, type<x0>, type<x1>, type<x2>, type<x3>) == f(f(f(type<x0>, type<x1>), type<x2>), type<x3>));
}

template <typename F>
constexpr void test_foldl_t(F f) {
    using detail::left_folds::variadic_t;
    BOOST_HANA_STATIC_ASSERT(variadic_t<>(f, type<x0>) == type<x0>);
    BOOST_HANA_STATIC_ASSERT(variadic_t<x1>(f, type<x0>) == f(type<x0>, type<x1>));
    BOOST_HANA_STATIC_ASSERT(variadic_t<x1, x2>(f, type<x0>) == f(f(type<x0>, type<x1>), type<x2>));
    BOOST_HANA_STATIC_ASSERT(variadic_t<x1, x2, x3>(f, type<x0>) == f(f(f(type<x0>, type<x1>), type<x2>), type<x3>));
}

int main() {
    test_foldl(detail::left_folds::variadic_unrolled, f);
    test_foldl(detail::left_folds::variadic_unrolled, g);

    test_foldl(detail::left_folds::variadic, f);
    test_foldl(detail::left_folds::variadic, g);

    test_foldl_t(f);
    test_foldl_t(g);
}
