/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <test/iso/integral_constant.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTANT_ASSERT(equal(
        integral_constant<Integral, int, 3>,
        integral<int, 3>
    ));

    IntegralConstant_isomorphism<Integral, int, int>();
    IntegralConstant_isomorphism<Integral, int, long>();
    IntegralConstant_isomorphism<Integral, short, long>();
}
