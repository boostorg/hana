/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <test/iso/integral_constant/comparable.hpp>
#include <test/iso/integral_constant/group.hpp>
#include <test/iso/integral_constant/integral_domain.hpp>
#include <test/iso/integral_constant/logical.hpp>
#include <test/iso/integral_constant/monoid.hpp>
#include <test/iso/integral_constant/orderable.hpp>
#include <test/iso/integral_constant/ring.hpp>
using namespace boost::hana;


int main() {
    IntegralConstant_comparable<BoostMplIntegralConstant, int, int>();
    IntegralConstant_orderable<BoostMplIntegralConstant, int, int>();
    IntegralConstant_logical<BoostMplIntegralConstant, int, int>();

    IntegralConstant_monoid<BoostMplIntegralConstant, int, int>();
    IntegralConstant_group<BoostMplIntegralConstant, int, int>();
    IntegralConstant_ring<BoostMplIntegralConstant, int, int>();
    IntegralConstant_integral_domain<BoostMplIntegralConstant, int, int>();
}
