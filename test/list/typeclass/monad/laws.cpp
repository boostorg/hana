/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/monad/laws.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [=](auto x) {
        return list(std::make_tuple(x));
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto g = f;

    BOOST_HANA_STATIC_ASSERT(Monad::laws::check(list(), x<0>, f, g));
    BOOST_HANA_STATIC_ASSERT(Monad::laws::check(list(x<0>), x<1>, f, g));
    BOOST_HANA_STATIC_ASSERT(Monad::laws::check(list(x<0>, x<1>), x<2>, f, g));
    BOOST_HANA_STATIC_ASSERT(Monad::laws::check(list(x<0>, x<1>, x<2>), x<3>, f, g));
}

int main() {
    test<List::mcd<void>>();
}
