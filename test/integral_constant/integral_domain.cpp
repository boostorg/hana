/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/enumerable.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/monoid.hpp>
#include <laws/ring.hpp>
using namespace boost::hana;


int main() {
    auto int_constants = tuple_c<int, -10, -2, 0, 1, 3, 4>;

    //////////////////////////////////////////////////////////////////////////
    // Enumerable, Monoid, Group, Ring, IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        // operators
        static_assert(has_operator<IntegralConstant<int>, decltype(plus)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(minus)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(negate)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(mult)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(quot)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(rem)>{}, "");

        // laws
        test::TestEnumerable<IntegralConstant<int>>{int_constants};
        test::TestMonoid<IntegralConstant<int>>{int_constants};
        test::TestGroup<IntegralConstant<int>>{int_constants};
        test::TestRing<IntegralConstant<int>>{int_constants};
        test::TestIntegralDomain<IntegralConstant<int>>{int_constants};
    }
}
