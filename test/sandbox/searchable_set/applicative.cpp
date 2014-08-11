/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/cnumber/constant.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
using namespace boost::hana;


// We need the comparison to be compile-time.
template <int i>
constexpr auto x = detail::cnumber<int, i>;

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});

    // ap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(singleton(f), singleton(x<0>)),
            singleton(f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(singleton(f), doubleton(x<0>, x<1>)),
            doubleton(f(x<0>), f(x<1>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(doubleton(f, g), singleton(x<0>)),
            doubleton(f(x<0>), g(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(doubleton(f, g), doubleton(x<0>, x<1>)),
            union_(doubleton(f(x<0>), f(x<1>)), doubleton(g(x<0>), g(x<1>)))
        ));
    }

    // lift
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            lift<SearchableSet>(x<0>),
            singleton(x<0>)
        ));
    }
}
