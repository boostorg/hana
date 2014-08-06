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
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return x;
};

template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    BOOST_HANA_CONSTANT_ASSERT(count(foldable(), f) == size_t<0>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>), f) == size_t<1>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>), f) == size_t<0>);

    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<1>), f) == size_t<2>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<0>), f) == size_t<1>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<1>), f) == size_t<1>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<0>), f) == size_t<0>);

    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<1>, long_<1>), f) == size_t<3>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<1>, long_<0>), f) == size_t<2>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<0>, long_<1>), f) == size_t<2>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<0>, long_<0>), f) == size_t<1>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<1>, long_<1>), f) == size_t<2>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<1>, long_<0>), f) == size_t<1>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<0>, long_<1>), f) == size_t<1>);
    BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<0>, long_<0>), f) == size_t<0>);


    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}), f) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}), f) == 0);

    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{1}), f) == 2);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{0}), f) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{1}), f) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{0}), f) == 0);

    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{1}, double{1}), f) == 3);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{1}, double{0}), f) == 2);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{0}, double{1}), f) == 2);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{0}, double{0}), f) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{1}, double{1}), f) == 2);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{1}, double{0}), f) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{0}, double{1}), f) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{0}, double{0}), f) == 0);
}

int main() {
    test<Foldable::mcd>();
    test<Foldable::unpack_mcd>();
}
