/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/left_folds/variadic_unrolled.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto variadic_unrolled = [](auto f, auto s, auto ...xs) {
    return detail::left_folds::variadic_unrolled(f, s, xs...);
};

template <typename Foldl>
constexpr void test_foldl(Foldl foldl) {
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, int_<1>) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, int_<1>, int_<2>) == list(list(int_<0>, int_<1>), int_<2>));
    BOOST_HANA_STATIC_ASSERT(foldl(list, int_<0>, int_<1>, int_<2>, int_<3>) == list(list(list(int_<0>, int_<1>), int_<2>), int_<3>));
}

int main() {
    test_foldl(variadic_unrolled);
}
