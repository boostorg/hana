/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable/lazy_foldr_mcd.hpp>

#include <boost/hana/detail/minimal/foldable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto foldable = detail::minimal::foldable<mcd>;

    // compile-time
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<0>)) == int_<0>);

    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<0>, int_<1>)) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<1>, int_<0>)) == int_<1>);

    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<0>, int_<1>, int_<2>)) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<1>, int_<0>, int_<2>)) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<1>, int_<2>, int_<0>)) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<2>, int_<1>, int_<0>)) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<2>, int_<0>, int_<1>)) == int_<2>);

    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<0>, int_<1>, int_<2>, int_<3>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<1>, int_<0>, int_<2>, int_<3>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<1>, int_<2>, int_<0>, int_<3>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<1>, int_<2>, int_<3>, int_<0>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<2>, int_<1>, int_<3>, int_<0>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<2>, int_<3>, int_<1>, int_<0>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<2>, int_<3>, int_<0>, int_<1>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<3>, int_<2>, int_<0>, int_<1>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<3>, int_<0>, int_<2>, int_<1>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<3>, int_<0>, int_<1>, int_<2>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<0>, int_<2>, int_<3>, int_<1>)) == int_<3>);
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int_<0>, int_<3>, int_<1>, int_<2>)) == int_<3>);


    // constexpr/runtime
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{0})) == int{0});

    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{0}, char{1})) == char{1});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{1}, char{0})) == int{1});

    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{0}, char{1}, long{2})) == long{2});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{1}, char{0}, long{2})) == long{2});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{1}, char{2}, long{0})) == char{2});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{2}, char{1}, long{0})) == int{2});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{2}, char{0}, long{1})) == int{2});

    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{0}, char{1}, long{2}, double{3})) == double{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{1}, char{0}, long{2}, double{3})) == double{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{1}, char{2}, long{0}, double{3})) == double{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{1}, char{2}, long{3}, double{0})) == long{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{2}, char{1}, long{3}, double{0})) == long{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{2}, char{3}, long{1}, double{0})) == char{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{2}, char{3}, long{0}, double{1})) == char{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{3}, char{2}, long{0}, double{1})) == int{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{3}, char{0}, long{2}, double{1})) == int{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{3}, char{0}, long{1}, double{2})) == int{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{0}, char{2}, long{3}, double{1})) == long{3});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{0}, char{3}, long{1}, double{2})) == char{3});


    // mixed
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{0}, char{1}, long_<2>)) == long{2});
    BOOST_HANA_STATIC_ASSERT(maximum(foldable(int{0}, long_<1>, char{2})) == char{2});
}

int main() {
    test<Foldable::lazy_foldr_mcd>();
}
