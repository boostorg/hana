/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/concept/integral_domain.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/integral_domain.hpp>
using namespace boost::hana;


int main() {
    test::TestIntegralDomain<int>{make<Tuple>(0,1,2,3,4,5)};
    test::TestIntegralDomain<long>{make<Tuple>(0l,1l,2l,3l,4l,5l)};

    // quot
    {
        static_assert(quot(6, 4) == 6 / 4, "");
        static_assert(quot(7, -3) == 7 / -3, "");
    }

    // rem
    {
        static_assert(rem(6, 4) == 6 % 4, "");
        static_assert(rem(7, -3) == 7 % -3, "");
    }
}
