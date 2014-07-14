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


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
    return std::make_tuple(xs...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto invalid = [](auto x) {
    return x.this_function_must_not_be_instantiated;
};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_STATIC_ASSERT(ap(lazy(f), lazy(x<0>)) == lazy(f(x<0>)));
    BOOST_HANA_STATIC_ASSERT(ap(lazy(f), lazy(x<0>), lazy(x<1>)) == lazy(f(x<0>, x<1>)));
    BOOST_HANA_STATIC_ASSERT(ap(lazy(f), lazy(x<0>), lazy(x<1>), lazy(x<2>)) == lazy(f(x<0>, x<1>, x<2>)));

    // The function is not applied.
    ap(lazy(invalid), lazy(x<0>));
    ap(lazy(invalid), lazy(x<0>), lazy(x<1>));
    ap(lazy(invalid), lazy(x<0>), lazy(x<1>), lazy(x<2>));
}
