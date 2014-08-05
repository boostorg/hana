/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = int_<i>;

BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

template <typename mcd>
void test() {
    constexpr auto list = detail::minimal::list<mcd>;

    constexpr struct { } invalid{};

    BOOST_HANA_CONSTANT_ASSERT(find(is(x<9>), list()) == nothing);

    BOOST_HANA_CONSTANT_ASSERT(find(is(x<9>), list(x<0>)) == nothing);
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<0>), list(x<0>)) == just(x<0>));
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<0>), list(x<0>, invalid)) == just(x<0>));
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<0>), list(x<0>, invalid, invalid)) == just(x<0>));

    BOOST_HANA_CONSTANT_ASSERT(find(is(x<9>), list(x<0>, x<1>)) == nothing);
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<1>), list(x<0>, x<1>)) == just(x<1>));
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<1>), list(x<0>, x<1>, invalid)) == just(x<1>));
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<1>), list(x<0>, x<1>, invalid, invalid)) == just(x<1>));

    BOOST_HANA_CONSTANT_ASSERT(find(is(x<9>), list(x<0>, x<1>, x<2>)) == nothing);
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<2>), list(x<0>, x<1>, x<2>)) == just(x<2>));
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<2>), list(x<0>, x<1>, x<2>, nothing)) == just(x<2>));
    BOOST_HANA_CONSTANT_ASSERT(find(is(x<2>), list(x<0>, x<1>, x<2>, nothing, nothing)) == just(x<2>));
}

int main() {
    test<List::mcd<void>>();
    (void)is;
}