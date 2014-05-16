/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


void test_Iterable() {
    BOOST_HANA_STATIC_ASSERT(is_empty(range(int_<0>, int_<0>)));
    BOOST_HANA_STATIC_ASSERT(!is_empty(range(int_<0>, int_<1>)));
    BOOST_HANA_STATIC_ASSERT(!is_empty(range(int_<0>, int_<2>)));

    BOOST_HANA_STATIC_ASSERT(head(range(int_<0>, int_<1>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(head(range(int_<0>, int_<2>)) == int_<0>);

    BOOST_HANA_STATIC_ASSERT(tail(range(int_<0>, int_<1>)) == range(int_<1>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(tail(range(int_<0>, int_<2>)) == range(int_<1>, int_<2>));
    BOOST_HANA_STATIC_ASSERT(tail(range(int_<0>, int_<3>)) == range(int_<1>, int_<3>));
}

void test_comparison() {
    BOOST_HANA_STATIC_ASSERT(range(int_<0>, int_<0>) == range(int_<0>, int_<0>));
    BOOST_HANA_STATIC_ASSERT(range(int_<0>, int_<0>) != range(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(range(int_<0>, int_<1>) != range(int_<0>, int_<0>));
    BOOST_HANA_STATIC_ASSERT(range(int_<0>, int_<1>) == range(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(range(int_<0>, int_<2>) != range(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(range(int_<0>, int_<2>) == range(int_<0>, int_<2>));
    BOOST_HANA_STATIC_ASSERT(range(int_<0>, int_<0>) == range(int_<2>, int_<2>));
}

constexpr struct {
    template <typename I>
    constexpr auto operator()(I i) const { return i + int_<1>; }
} f{};
void test_Functor() {
    BOOST_HANA_STATIC_ASSERT(fmap(f, range(int_<0>, int_<0>)) == list());
    BOOST_HANA_STATIC_ASSERT(fmap(f, range(int_<0>, int_<1>)) == list(f(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, range(int_<0>, int_<2>)) == list(f(int_<0>), f(int_<1>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, range(int_<0>, int_<3>)) == list(f(int_<0>), f(int_<1>), f(int_<2>)));
}

void test_Foldable() {
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, range(int_<1>, int_<1>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, range(int_<1>, int_<2>)) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, range(int_<1>, int_<3>)) == list(list(int_<0>, int_<1>), int_<2>));
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, range(int_<1>, int_<4>)) == list(list(list(int_<0>, int_<1>), int_<2>), int_<3>));

    BOOST_HANA_STATIC_ASSERT(foldr(list, int_<0>, range(int_<0>, int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldr(list, int_<1>, range(int_<0>, int_<1>)) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldr(list, int_<2>, range(int_<0>, int_<2>)) == list(int_<0>, list(int_<1>, int_<2>)));
    BOOST_HANA_STATIC_ASSERT(foldr(list, int_<3>, range(int_<0>, int_<3>)) == list(int_<0>, list(int_<1>, list(int_<2>, int_<3>))));
}

int main() {
    test_Iterable();
    test_comparison();
    test_Functor();
    test_Foldable();
}
