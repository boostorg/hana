/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::number<>(i);

BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

struct invalid { };

template <bool b>
BOOST_HANA_CONSTEXPR_LAMBDA auto pred = [](auto) {
    return bool_<b>; // we need a compile-time logical
};

int main() {
    // maybe
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(maybe(x<0>, invalid{}, nothing), x<0>));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(maybe(invalid{}, f, just(x<0>)), f(x<0>)));
    }

    // is_nothing
    {
        BOOST_HANA_CONSTANT_ASSERT( is_nothing(nothing));
        BOOST_HANA_CONSTANT_ASSERT(!is_nothing(just(invalid{})));
    }

    // is_just
    {
        BOOST_HANA_CONSTANT_ASSERT( is_just(just(invalid{})));
        BOOST_HANA_CONSTANT_ASSERT(!is_just(nothing));
    }

    // from_just
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(from_just(just(x<0>)), x<0>));
        // from_just(nothing);
    }

    // from_maybe
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(from_maybe(x<0>, nothing), x<0>));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(from_maybe(invalid{}, just(x<1>)), x<1>));
    }

    // only_when
    {
        BOOST_HANA_CONSTEXPR_ASSERT(only_when(pred<true>, f, x<0>) == just(f(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(only_when(pred<false>, f, x<0>) == nothing);
        BOOST_HANA_CONSTANT_ASSERT(only_when(pred<false>, invalid{}, x<0>) == nothing);
    }
}
