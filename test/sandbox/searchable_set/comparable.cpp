/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/laws/comparable.hpp>
#include <test/numeric/comparable.hpp>
using namespace boost::hana;


template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto x = test::numeric(i);

int main() {
    // equal
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(singleton(x<0>), singleton(x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(singleton(x<0>), singleton(x<1>))));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(singleton(x<0>), doubleton(x<0>, x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(singleton(x<0>), doubleton(x<0>, x<1>))));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(singleton(x<0>), doubleton(x<1>, x<1>))));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(doubleton(x<0>, x<1>), doubleton(x<0>, x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(doubleton(x<0>, x<1>), doubleton(x<1>, x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(doubleton(x<0>, x<1>), doubleton(x<0>, x<0>))));
        BOOST_HANA_CONSTEXPR_ASSERT(not_(equal(doubleton(x<0>, x<1>), doubleton(x<3>, x<4>))));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Comparable_laws(
            singleton(x<0>),
            singleton(x<1>),
            doubleton(x<0>, x<1>),
            doubleton(x<0>, x<3>),
            doubleton(x<0>, x<0>)
        ));
    }
}
