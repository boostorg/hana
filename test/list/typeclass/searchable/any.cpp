/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/integral.hpp>
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

    BOOST_HANA_CONSTANT_ASSERT(!any(list(), is(x<9>)));

    BOOST_HANA_CONSTANT_ASSERT(!any(list(x<0>), is(x<9>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>), is(x<0>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>, invalid), is(x<0>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>, invalid, invalid), is(x<0>)));

    BOOST_HANA_CONSTANT_ASSERT(!any(list(x<0>, x<1>), is(x<9>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>, x<1>), is(x<1>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>, x<1>, invalid), is(x<1>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>, x<1>, invalid, invalid), is(x<1>)));

    BOOST_HANA_CONSTANT_ASSERT(!any(list(x<0>, x<1>, x<2>), is(x<9>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>, x<1>, x<2>), is(x<2>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>, x<1>, x<2>, nothing), is(x<2>)));
    BOOST_HANA_CONSTANT_ASSERT( any(list(x<0>, x<1>, x<2>, nothing, nothing), is(x<2>)));
}

int main() {
    test<List::mcd<void>>();
    (void)is;
}
