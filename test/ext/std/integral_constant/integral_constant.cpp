/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>

#include <test/iso/integral_constant.hpp>
using namespace boost::hana;


int main() {
    IntegralConstant_isomorphism<ext::std::IntegralConstant, int, long>();
}
