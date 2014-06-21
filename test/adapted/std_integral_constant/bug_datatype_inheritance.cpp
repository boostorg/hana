/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_integral_constant.hpp>

#include <boost/hana/core.hpp>

#include <type_traits>
using namespace boost::hana;


struct inherit_simple : std::integral_constant<int, 3> { };
struct inherit_no_default : std::integral_constant<int, 3> {
    inherit_no_default() = delete;
};

struct incomplete;
struct empty { };
struct non_pod { virtual ~non_pod() { } };

int main() {
    static_assert(std::is_same<datatype_t<inherit_simple>, StdIntegralConstant>{}, "");
    static_assert(std::is_same<datatype_t<inherit_no_default>, StdIntegralConstant>{}, "");
    static_assert(std::is_same<datatype_t<std::is_pointer<int*>>, StdIntegralConstant>{}, "");

    static_assert(!std::is_same<datatype_t<incomplete>, StdIntegralConstant>{}, "");
    static_assert(!std::is_same<datatype_t<empty>, StdIntegralConstant>{}, "");
    static_assert(!std::is_same<datatype_t<non_pod>, StdIntegralConstant>{}, "");
    static_assert(!std::is_same<datatype_t<void>, StdIntegralConstant>{}, "");
}
