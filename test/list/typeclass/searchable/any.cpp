/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/static_assert.hpp>
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

    BOOST_HANA_STATIC_ASSERT(!any(is(x<9>), list()));

    BOOST_HANA_STATIC_ASSERT(!any(is(x<9>), list(x<0>)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<0>), list(x<0>)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<0>), list(x<0>, invalid)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<0>), list(x<0>, invalid, invalid)));

    BOOST_HANA_STATIC_ASSERT(!any(is(x<9>), list(x<0>, x<1>)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<1>), list(x<0>, x<1>)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<1>), list(x<0>, x<1>, invalid)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<1>), list(x<0>, x<1>, invalid, invalid)));

    BOOST_HANA_STATIC_ASSERT(!any(is(x<9>), list(x<0>, x<1>, x<2>)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<2>), list(x<0>, x<1>, x<2>)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<2>), list(x<0>, x<1>, x<2>, nothing)));
    BOOST_HANA_STATIC_ASSERT( any(is(x<2>), list(x<0>, x<1>, x<2>, nothing, nothing)));
}

int main() {
    test<List::mcd<void>>();
}
