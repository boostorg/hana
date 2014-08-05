/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    auto p = list; // tests are unreadable otherwise. mnemonic: pair

    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p())) == list());

    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p(), p())) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(unzip(list(p(x<0>, x<2>), p(x<1>, x<3>, x<4>))) == list(p(x<0>, x<1>), p(x<2>, x<3>)));

    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p(),     p(),     p()))     == list());
    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p(x<0>), p(),     p()))     == list());
    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p(),     p(x<1>), p()))     == list());
    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p(),     p(),     p(x<2>))) == list());
    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p(x<0>), p(x<1>), p()))     == list());
    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p(),     p(x<1>), p(x<2>))) == list());
    BOOST_HANA_CONSTANT_ASSERT(unzip(list(p(x<0>), p(),     p(x<2>))) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(unzip(list(p(x<0>), p(x<1>), p(x<2>))) == list(p(x<0>, x<1>, x<2>)));

    BOOST_HANA_CONSTEXPR_ASSERT(unzip(list(p(x<0>, x<3>), p(x<1>, x<4>), p(x<2>, x<5>))) == list(p(x<0>, x<1>, x<2>), p(x<3>, x<4>, x<5>)));
}

int main() {
    test<List::mcd<void>>();
}
