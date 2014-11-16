/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

// instances
#include <test/auto/constant.hpp>
#include <test/auto/integral_constant.hpp>

#include <type_traits>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <typename T>
    auto instances<ext::std::IntegralConstant<T>> = tuple(
        type<Constant>,
        type<IntegralConstant>
    );

    template <typename T>
    auto objects<ext::std::IntegralConstant<T>> = tuple(
        ::std::integral_constant<T, 0>{},
        ::std::integral_constant<T, 1>{},
        ::std::integral_constant<T, 2>{},
        ::std::integral_constant<T, 3>{}
    );
}}}


int main() {
    test::check_datatype<ext::std::IntegralConstant<int>>();

    // interoperation with Integral
    {
        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 1>{} == int_<1>);
        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 1>{} == long_<1>);

        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 2>{} != int_<3>);
    }

    // Constant
    {
        // value
        {
            static_assert(value(std::integral_constant<int, 0>{}) == 0, "");
            static_assert(value(std::integral_constant<int, 1>{}) == 1, "");
        }
    }
}
