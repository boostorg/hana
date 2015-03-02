/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/orderable.hpp>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto int_constants = tuple_c<int, -10, -2, 0, 1, 3, 4>;

    //////////////////////////////////////////////////////////////////////////
    // Comparable and Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        // operators
        static_assert(has_operator<IntegralConstant<int>, decltype(equal)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(not_equal)>{}, "");

        static_assert(has_operator<IntegralConstant<int>, decltype(less)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(less_equal)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(greater)>{}, "");
        static_assert(has_operator<IntegralConstant<int>, decltype(greater_equal)>{}, "");

        // laws
        test::TestComparable<IntegralConstant<int>>{int_constants};
        test::TestOrderable<IntegralConstant<int>>{int_constants};
    }
}
