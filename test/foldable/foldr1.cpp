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


BOOST_HANA_CONSTEXPR_LAMBDA auto tup = [](auto ...xs) {
    return std::make_tuple(xs...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto xs) {
    return std::tuple_cat(tup(x), xs);
};

template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(foldr1(foldable(tup(1)), f) == tup(1));
    BOOST_HANA_CONSTEXPR_ASSERT(foldr1(foldable(1, tup('2')), f) == f(1, tup('2')));
    BOOST_HANA_CONSTEXPR_ASSERT(foldr1(foldable(1, '2', tup(3.3)), f) == f(1, f('2', tup(3.3))));
    BOOST_HANA_CONSTEXPR_ASSERT(foldr1(foldable(1, '2', 3.3, tup(4.4f)), f) == f(1, f('2', f(3.3, tup(4.4f)))));

    BOOST_HANA_CONSTEXPR_ASSERT(foldr1(foldable(tup(int_<1>)), f) == tup(int_<1>));
    BOOST_HANA_CONSTEXPR_ASSERT(foldr1(foldable(int_<1>, tup(int_<2>)), f) == f(int_<1>, tup(int_<2>)));
    BOOST_HANA_CONSTEXPR_ASSERT(foldr1(foldable(int_<1>, int_<2>, tup(int_<3>)), f) == f(int_<1>, f(int_<2>, tup(int_<3>))));
    BOOST_HANA_CONSTEXPR_ASSERT(foldr1(foldable(int_<1>, int_<2>, int_<3>, tup(int_<4>)), f) == f(int_<1>, f(int_<2>, f(int_<3>, tup(int_<4>)))));
}

int main() {
    test<Foldable::mcd>();
    test<Foldable::unpack_mcd>();
}
