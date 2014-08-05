/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/foldable.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto xs, auto x) {
    return std::tuple_cat(xs, std::make_tuple(x));
};

template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    {
    constexpr auto s = std::make_tuple(1);
    BOOST_HANA_CONSTEXPR_ASSERT(foldl1(foldable(s), f) == s);
    BOOST_HANA_CONSTEXPR_ASSERT(foldl1(foldable(s, '2'), f) == f(s, '2'));
    BOOST_HANA_CONSTEXPR_ASSERT(foldl1(foldable(s, '2', 3.3), f) == f(f(s, '2'), 3.3));
    BOOST_HANA_CONSTEXPR_ASSERT(foldl1(foldable(s, '2', 3.3, 4.4f), f) == f(f(f(s, '2'), 3.3), 4.4f));
    }

    {
    constexpr auto s = std::make_tuple(int_<1>);
    BOOST_HANA_CONSTEXPR_ASSERT(foldl1(foldable(s), f) == s);
    BOOST_HANA_CONSTEXPR_ASSERT(foldl1(foldable(s, int_<2>), f) == f(s, int_<2>));
    BOOST_HANA_CONSTEXPR_ASSERT(foldl1(foldable(s, int_<2>, int_<3>), f) == f(f(s, int_<2>), int_<3>));
    BOOST_HANA_CONSTEXPR_ASSERT(foldl1(foldable(s, int_<2>, int_<3>, int_<4>), f) == f(f(f(s, int_<2>), int_<3>), int_<4>));
    }
}

int main() {
    test<Foldable::mcd>();
}
