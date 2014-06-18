/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "minimal_foldable.hpp"
#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto xs, auto x) {
    return std::tuple_cat(xs, std::make_tuple(x));
};

BOOST_HANA_CONSTEXPR_LAMBDA auto lf = [](auto xs, auto x) {
    return f(xs(), x());
};

int main() {
    constexpr std::tuple<> s{};
    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable()) == s);
    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable(1)) == f(s, 1));
    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable(1, '2')) == f(f(s, 1), '2'));
    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable(1, '2', 3.3)) == f(f(f(s, 1), '2'), 3.3));
    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable(1, '2', 3.3, 4.4f)) == f(f(f(f(s, 1), '2'), 3.3), 4.4f));

    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable(int_<0>)) == f(s, int_<0>));
    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable(int_<0>, int_<1>)) == f(f(s, int_<0>), int_<1>));
    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable(int_<0>, int_<1>, int_<2>)) == f(f(f(s, int_<0>), int_<1>), int_<2>));
    BOOST_HANA_STATIC_ASSERT(lazy_foldl(lf, s, foldable(int_<0>, int_<1>, int_<2>, int_<3>)) == f(f(f(f(s, int_<0>), int_<1>), int_<2>), int_<3>));
}
