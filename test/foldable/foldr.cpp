/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable/mcd.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/foldable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto xs) {
    return std::tuple_cat(std::make_tuple(x), xs);
};

template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    constexpr std::tuple<> s;
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable()) == s);
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable(1)) == f(1, s));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable(1, '2')) == f(1, f('2', s)));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable(1, '2', 3.3)) == f(1, f('2', f(3.3, s))));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable(1, '2', 3.3, 4.4f)) == f(1, f('2', f(3.3, f(4.4f, s)))));

    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable(int_<0>)) == f(int_<0>, s));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable(int_<0>, int_<1>)) == f(int_<0>, f(int_<1>, s)));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable(int_<0>, int_<1>, int_<2>)) == f(int_<0>, f(int_<1>, f(int_<2>, s))));
    BOOST_HANA_STATIC_ASSERT(foldr(f, s, foldable(int_<0>, int_<1>, int_<2>, int_<3>)) == f(int_<0>, f(int_<1>, f(int_<2>, f(int_<3>, s)))));
}

int main() {
    test<Foldable::mcd>();
}
