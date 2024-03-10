// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/traits_expr.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/type.hpp>
namespace hana   = boost::hana;
namespace traits = boost::hana::traits;

enum Enumeration {};
struct Structure {
};
constexpr auto e = hana::type_c<Enumeration>;
constexpr auto s = hana::type_c<Structure>;

int main()
{
    // We just make sure that they compile. If the forwarding to `std::` is
    // well done, it is the job of `std::` to return the right thing.

    ///////////////////////
    // Type traits predict expression
    ///////////////////////
    // Primary type categories
    constexpr auto is_integral_OR_is_float_point = traits::is_integral | traits::is_floating_point;
    static_assert(is_integral_OR_is_float_point(hana::type_c<int>), "");
    static_assert(is_integral_OR_is_float_point(hana::type_c<double>), "");

    constexpr auto not_integral = !traits::is_integral;
    static_assert(not_integral(hana::type_c<double>), "");
    static_assert(not_integral(hana::type_c<void>), "");

    constexpr auto is_integral_and_is_trivial = traits::is_integral & traits::is_trivial;
    static_assert(is_integral_and_is_trivial(hana::type_c<int>), "");
    constexpr auto not_integral_but_is_trivial = (!traits::is_integral) & traits::is_trivial;
    static_assert(not_integral_but_is_trivial(hana::type_c<double>), "");
}
