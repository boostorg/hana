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
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return x;
};

int main() {
    // compile-time
    BOOST_HANA_STATIC_ASSERT(count(f, foldable()) == size_t<0>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<1>)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<0>)) == size_t<0>);

    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<1>, char_<1>)) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<1>, char_<0>)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<0>, char_<1>)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<0>, char_<0>)) == size_t<0>);

    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<1>, char_<1>, long_<1>)) == size_t<3>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<1>, char_<1>, long_<0>)) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<1>, char_<0>, long_<1>)) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<1>, char_<0>, long_<0>)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<0>, char_<1>, long_<1>)) == size_t<2>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<0>, char_<1>, long_<0>)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<0>, char_<0>, long_<1>)) == size_t<1>);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int_<0>, char_<0>, long_<0>)) == size_t<0>);


    // constexpr/runtime
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{1})) == 1);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{0})) == 0);

    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{1}, char{1})) == 2);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{1}, char{0})) == 1);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{0}, char{1})) == 1);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{0}, char{0})) == 0);

    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{1}, char{1}, double{1})) == 3);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{1}, char{1}, double{0})) == 2);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{1}, char{0}, double{1})) == 2);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{1}, char{0}, double{0})) == 1);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{0}, char{1}, double{1})) == 2);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{0}, char{1}, double{0})) == 1);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{0}, char{0}, double{1})) == 1);
    BOOST_HANA_STATIC_ASSERT(count(f, foldable(int{0}, char{0}, double{0})) == 0);
}
