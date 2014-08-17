/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <test/iso/integral_constant/integral_domain.hpp>
using namespace boost::hana;


int main() {
    IntegralConstant_integral_domain<Integral, int, short>();
    IntegralConstant_integral_domain<Integral, int, int>();
    IntegralConstant_integral_domain<Integral, int, long>();
}
