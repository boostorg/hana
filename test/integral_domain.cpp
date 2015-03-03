/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>
#include <test/auto/integral_domain.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <> auto objects<int> = make<Tuple>(0,1,2,3,4,5);
    template <> auto objects<long> = make<Tuple>(0l,1l,2l,3l,4l,5l);
}}}


int main() {
    test::laws<IntegralDomain, int>();
    test::laws<IntegralDomain, long>();

    using T = int;
    constexpr T x = 6, y = 4;

    // quot
    {
        BOOST_HANA_CONSTEXPR_CHECK(quot(x, y) == x / y);
    }

    // mod
    {
        BOOST_HANA_CONSTEXPR_CHECK(mod(x, y) == x % y);
    }
}
