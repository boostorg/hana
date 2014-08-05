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


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...x) {
    return std::make_tuple(x...);
};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(unpack(list(), f) == f());
    BOOST_HANA_CONSTEXPR_ASSERT(unpack(list(x<0>), f) == f(x<0>));
    BOOST_HANA_CONSTEXPR_ASSERT(unpack(list(x<0>, x<1>), f) == f(x<0>, x<1>));
    BOOST_HANA_CONSTEXPR_ASSERT(unpack(list(x<0>, x<1>, x<2>), f) == f(x<0>, x<1>, x<2>));
}
