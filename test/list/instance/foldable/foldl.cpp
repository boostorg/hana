/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto y) {
    return std::make_tuple(x, y);
};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto s = x<0>;
    BOOST_HANA_CONSTEXPR_ASSERT(foldl(list(), s, f) == s);
    BOOST_HANA_CONSTEXPR_ASSERT(foldl(list(x<1>), s, f) == f(s, x<1>));
    BOOST_HANA_CONSTEXPR_ASSERT(foldl(list(x<1>, x<2>), s, f) == f(f(s, x<1>), x<2>));
    BOOST_HANA_CONSTEXPR_ASSERT(foldl(list(x<1>, x<2>, x<3>), s, f) == f(f(f(s, x<1>), x<2>), x<3>));
}
