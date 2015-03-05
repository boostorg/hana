/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/constant.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    auto constants = make<Tuple>(
        std::integral_constant<int, -10>{},
        std::integral_constant<int, -2>{},
        std::integral_constant<int, 0>{},
        std::integral_constant<int, 1>{},
        std::integral_constant<int, 3>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    {
        // value
        static_assert(value(std::integral_constant<int, 0>{}) == 0, "");
        static_assert(value(std::integral_constant<int, 1>{}) == 1, "");
        static_assert(value(std::integral_constant<int, 3>{}) == 3, "");

        // laws
        test::TestConstant<ext::std::IntegralConstant<int>>{constants};
    }
}
