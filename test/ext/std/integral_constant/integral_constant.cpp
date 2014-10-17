/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>

#include <test/iso/integral_constant/comparable.hpp>
#include <test/iso/integral_constant/enumerable.hpp>
#include <test/iso/integral_constant/group.hpp>
#include <test/iso/integral_constant/integral_domain.hpp>
#include <test/iso/integral_constant/logical.hpp>
#include <test/iso/integral_constant/monoid.hpp>
#include <test/iso/integral_constant/orderable.hpp>
#include <test/iso/integral_constant/ring.hpp>
using namespace boost::hana;


int main() {
    IntegralConstant_comparable<StdIntegralConstant, int, int>();
    IntegralConstant_orderable<StdIntegralConstant, int, int>();
    IntegralConstant_logical<StdIntegralConstant, int, int>();
    IntegralConstant_enumerable<StdIntegralConstant, int>();

    IntegralConstant_monoid<StdIntegralConstant, int, int>();
    IntegralConstant_group<StdIntegralConstant, int, int>();
    IntegralConstant_ring<StdIntegralConstant, int, int>();
    IntegralConstant_integral_domain<StdIntegralConstant, int, int>();
}
