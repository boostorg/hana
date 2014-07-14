/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include "../comparable.hpp"
#include <tuple>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return lazy(std::make_tuple(x));
};

int main() {
    BOOST_HANA_STATIC_ASSERT(bind(lazy(x<0>), f) == f(x<0>));
    BOOST_HANA_STATIC_ASSERT(bind(lazy(x<1>), f) == f(x<1>));
}
