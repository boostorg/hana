/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return std::make_tuple(x);
};

struct invalid { };

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <bool b>
BOOST_HANA_CONSTEXPR_LAMBDA auto pred = [](auto) {
    return bool_<b>; // we need a compile-time logical
};

int main() {
    BOOST_HANA_STATIC_ASSERT(only_when(pred<true>, f, x<0>) == just(f(x<0>)));
    BOOST_HANA_STATIC_ASSERT(only_when(pred<false>, f, x<0>) == nothing);
    BOOST_HANA_STATIC_ASSERT(only_when(pred<false>, invalid{}, x<0>) == nothing);
}
