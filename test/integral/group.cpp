/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <test/iso/integral_constant/group.hpp>
using namespace boost::hana;


int main() {
    IntegralConstant_group<Integral, int, short>();
    IntegralConstant_group<Integral, int, int>();
    IntegralConstant_group<Integral, int, long>();
}
