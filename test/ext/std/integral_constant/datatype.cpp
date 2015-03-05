/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/constant.hpp>

#include <type_traits>
using namespace boost::hana;


struct inherit_simple : std::integral_constant<int, 3> { };
struct inherit_no_default : std::integral_constant<int, 3> {
    inherit_no_default() = delete;
};

struct incomplete;
struct empty_type { };
struct non_pod { virtual ~non_pod() { } };


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Make sure the data type is detected properly
    //////////////////////////////////////////////////////////////////////////
    {
        static_assert(std::is_same<
            datatype_t<inherit_simple>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(std::is_same<
            datatype_t<inherit_no_default>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(std::is_same<
            datatype_t<std::is_pointer<int*>>,
            ext::std::IntegralConstant<bool>
        >{}, "");

        static_assert(!std::is_same<
            datatype_t<incomplete>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(!std::is_same<
            datatype_t<empty_type>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(!std::is_same<
            datatype_t<non_pod>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(!std::is_same<
            datatype_t<void>,
            ext::std::IntegralConstant<int>
        >{}, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // Interoperation with IntegralConstant
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 1>{} == int_<1>);
        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 1>{} == long_<1>);

        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 2>{} != int_<3>);
    }
}
