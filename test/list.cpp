/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <functional>
using namespace boost::hana;


void test_Iterable() {
    BOOST_HANA_STATIC_ASSERT(is_empty(list()));
    BOOST_HANA_STATIC_ASSERT(!is_empty(list(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(!is_empty(list(int_<0>, int_<1>)));

    BOOST_HANA_STATIC_ASSERT(head(list(int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(head(list(int_<0>, int_<1>)) == int_<0>);

    BOOST_HANA_STATIC_ASSERT(tail(list(int_<0>)) == list());
    BOOST_HANA_STATIC_ASSERT(tail(list(int_<0>, int_<1>)) == list(int_<1>));
    BOOST_HANA_STATIC_ASSERT(tail(list(int_<0>, int_<1>, int_<2>)) == list(int_<1>, int_<2>));
}

void test_comparison() {
    BOOST_HANA_STATIC_ASSERT(list() == list());
    BOOST_HANA_STATIC_ASSERT(list(int_<0>) != list());
    BOOST_HANA_STATIC_ASSERT(list() != list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(list(int_<0>) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(list(int_<0>, int_<1>) != list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(list(int_<0>, int_<1>) == list(int_<0>, int_<1>));

    BOOST_HANA_STATIC_ASSERT(list(0) == list(0));
    BOOST_HANA_STATIC_ASSERT(list(0, 1) != list(0, 2));
    BOOST_HANA_STATIC_ASSERT(list(0, 1, 2) == list(0, 1, 2));
}

constexpr struct {
    template <typename I>
    constexpr auto operator()(I i) const { return i + int_<1>; }
} f{};
void test_Functor() {
    BOOST_HANA_STATIC_ASSERT(fmap(f, list()) == list());
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(int_<1>)) == list(f(int_<1>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(int_<1>, int_<2>)) == list(f(int_<1>), f(int_<2>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, list(int_<1>, int_<2>, int_<3>)) == list(f(int_<1>), f(int_<2>), f(int_<3>)));
}

template <int ...i>
constexpr auto list_c = list(int_<i>...);

void test_zip_with() {
    BOOST_HANA_STATIC_ASSERT(zip_with(std::plus<>{}, list(), list()) == list());
    BOOST_HANA_STATIC_ASSERT(zip_with(std::plus<>{}, list_c<1>, list()) == list());
    BOOST_HANA_STATIC_ASSERT(zip_with(std::plus<>{}, list(), list_c<3>) == list());
    BOOST_HANA_STATIC_ASSERT(zip_with(std::plus<>{}, list_c<1>, list_c<3>) == list_c<1 + 3>);
    BOOST_HANA_STATIC_ASSERT(zip_with(std::plus<>{}, list_c<1, 2>, list_c<3, 4>) == list_c<1 + 3, 2 + 4>);
    BOOST_HANA_STATIC_ASSERT(zip_with(std::plus<>{}, list_c<1, 2, 3, 4>, list_c<5, 6, 7>) == list_c<1 + 5, 2 + 6, 3 + 7>);
}

void test_zip() {
    BOOST_HANA_STATIC_ASSERT(zip(list(), list()) == list());
    BOOST_HANA_STATIC_ASSERT(zip(list_c<0>, list_c<1>) == list(list_c<0, 1>));
    BOOST_HANA_STATIC_ASSERT(zip(list_c<0>, list_c<1>, list_c<2>) == list(list_c<0, 1, 2>));
    BOOST_HANA_STATIC_ASSERT(zip(list_c<0, 3>, list_c<1, 4>, list_c<2, 5, 8>) == list(list_c<0, 1, 2>, list_c<3, 4, 5>));
}

void test_cons() {
    BOOST_HANA_STATIC_ASSERT(cons(int_<0>, list()) == list_c<0>);
    BOOST_HANA_STATIC_ASSERT(cons(int_<0>, list_c<1>) == list_c<0, 1>);
    BOOST_HANA_STATIC_ASSERT(cons(int_<0>, list_c<1, 2>) == list_c<0, 1, 2>);
}

void test_snoc() {
    BOOST_HANA_STATIC_ASSERT(snoc(list(), int_<0>) == list_c<0>);
    BOOST_HANA_STATIC_ASSERT(snoc(list_c<0>, int_<1>) == list_c<0, 1>);
    BOOST_HANA_STATIC_ASSERT(snoc(list_c<0, 1>, int_<2>) == list_c<0, 1, 2>);
}

void test_take() {
    BOOST_HANA_STATIC_ASSERT(take(int_<0>, list()) == list());
    BOOST_HANA_STATIC_ASSERT(take(int_<1>, list()) == list());
    BOOST_HANA_STATIC_ASSERT(take(int_<2>, list()) == list());

    BOOST_HANA_STATIC_ASSERT(take(int_<0>, list_c<0>) == list());
    BOOST_HANA_STATIC_ASSERT(take(int_<1>, list_c<0>) == list_c<0>);
    BOOST_HANA_STATIC_ASSERT(take(int_<2>, list_c<0>) == list_c<0>);
    BOOST_HANA_STATIC_ASSERT(take(int_<3>, list_c<0>) == list_c<0>);

    BOOST_HANA_STATIC_ASSERT(take(int_<0>, list_c<0, 1>) == list());
    BOOST_HANA_STATIC_ASSERT(take(int_<1>, list_c<0, 1>) == list_c<0>);
    BOOST_HANA_STATIC_ASSERT(take(int_<2>, list_c<0, 1>) == list_c<0, 1>);
    BOOST_HANA_STATIC_ASSERT(take(int_<3>, list_c<0, 1>) == list_c<0, 1>);
}

constexpr struct _odd {
    template <typename I>
    constexpr auto operator()(I i) const { return i % int_<2> != int_<0>; }
} odd{};
void test_take_while() {
    BOOST_HANA_STATIC_ASSERT(take_while(odd, list_c<>) == list_c<>);
    BOOST_HANA_STATIC_ASSERT(take_while(odd, list_c<0>) == list_c<>);
    BOOST_HANA_STATIC_ASSERT(take_while(odd, list_c<0, 2>) == list_c<>);

    BOOST_HANA_STATIC_ASSERT(take_while(odd, list_c<0, 1, 3>) == list_c<>);
    BOOST_HANA_STATIC_ASSERT(take_while(odd, list_c<1, 0, 3>) == list_c<1>);
    BOOST_HANA_STATIC_ASSERT(take_while(odd, list_c<1, 3, 0>) == list_c<1, 3>);
    BOOST_HANA_STATIC_ASSERT(take_while(odd, list_c<1, 3, 0, 2>) == list_c<1, 3>);
}

void test_reverse() {
    BOOST_HANA_STATIC_ASSERT(reverse(list_c<>) == list_c<>);
    BOOST_HANA_STATIC_ASSERT(reverse(list_c<0>) == list_c<0>);
    BOOST_HANA_STATIC_ASSERT(reverse(list_c<0, 1>) == list_c<1, 0>);
    BOOST_HANA_STATIC_ASSERT(reverse(list_c<0, 1, 2>) == list_c<2, 1, 0>);
}

void test_filter() {
    BOOST_HANA_STATIC_ASSERT(filter(odd, list_c<>) == list_c<>);
    BOOST_HANA_STATIC_ASSERT(filter(odd, list_c<0>) == list_c<>);
    BOOST_HANA_STATIC_ASSERT(filter(odd, list_c<0, 1>) == list_c<1>);
    BOOST_HANA_STATIC_ASSERT(filter(odd, list_c<0, 1, 2>) == list_c<1>);
    BOOST_HANA_STATIC_ASSERT(filter(odd, list_c<0, 1, 2, 3>) == list_c<1, 3>);
}

void test_concat() {
    BOOST_HANA_STATIC_ASSERT(concat() == list_c<>);
    BOOST_HANA_STATIC_ASSERT(concat(list_c<>) == list_c<>);
    BOOST_HANA_STATIC_ASSERT(concat(list_c<0>, list_c<>) == list_c<0>);
    BOOST_HANA_STATIC_ASSERT(concat(list_c<0>, list_c<1>) == list_c<0, 1>);
    BOOST_HANA_STATIC_ASSERT(concat(list_c<>, list_c<0>) == list_c<0>);
    BOOST_HANA_STATIC_ASSERT(concat(list_c<0>, list_c<1>, list_c<2, 3>) == list_c<0, 1, 2, 3>);
}

int main() {
    test_Iterable();
    test_comparison();
    test_Functor();
    test_zip_with();
    test_zip();
    test_cons();
    test_snoc();
    test_take();
    test_take_while();
    test_reverse();
    test_filter();
    test_concat();
}
