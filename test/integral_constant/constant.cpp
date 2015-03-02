/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/constant.hpp>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    {
        // value
        static_assert(value(integral_constant<int, 0>) == 0, "");
        static_assert(value(integral_constant<int, 1>) == 1, "");

        // laws
        test::TestConstant<IntegralConstant<int>>{tuple_c<int, -10, -2, 0, 1, 3, 4>};
        test::TestConstant<IntegralConstant<bool>>{tuple_c<bool, true, false>};
    }
}
