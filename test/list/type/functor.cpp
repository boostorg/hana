/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2;

constexpr struct {
    constexpr auto operator()(Type<x0>) const { return 0; }
    constexpr auto operator()(Type<x1>) const { return 1; }
    constexpr auto operator()(Type<x2>) const { return 2; }
} fv{};

void test_values() {
    BOOST_HANA_STATIC_ASSERT(fmap(fv, list_t<>) == list());
    BOOST_HANA_STATIC_ASSERT(fmap(fv, list_t<x0>) == list(0));
    BOOST_HANA_STATIC_ASSERT(fmap(fv, list_t<x0, x1>) == list(0, 1));
    BOOST_HANA_STATIC_ASSERT(fmap(fv, list_t<x0, x1, x2>) == list(0, 1, 2));
}


template <typename ...> struct _ft { };
constexpr auto ft = lift<_ft>;

void test_types() {
    BOOST_HANA_STATIC_ASSERT(fmap(ft, list_t<>) == list_t<>);
    BOOST_HANA_STATIC_ASSERT(fmap(ft, list_t<x0>) == list_t<_ft<x0>>);
    BOOST_HANA_STATIC_ASSERT(fmap(ft, list_t<x0, x1>) == list_t<_ft<x0>, _ft<x1>>);
    BOOST_HANA_STATIC_ASSERT(fmap(ft, list_t<x0, x1, x2>) == list_t<_ft<x0>, _ft<x1>, _ft<x2>>);
}


int main() {
    test_values();
    test_types();
}
