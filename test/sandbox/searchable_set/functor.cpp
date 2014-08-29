/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp> //! @todo find a way to remove this

#include <test/injection.hpp>
#include <test/numeric/comparable.hpp>
using namespace boost::hana;


template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto x = test::numeric(i);

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // fmap
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            fmap(singleton(x<0>), f),
            singleton(f(x<0>))
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            fmap(doubleton(x<0>, x<1>), f),
            doubleton(f(x<0>), f(x<1>))
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            fmap(doubleton(x<0>, x<0>), f),
            singleton(f(x<0>))
        ));
    }
}
