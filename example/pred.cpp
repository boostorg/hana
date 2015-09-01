/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/pred.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::pred(hana::int_c<4>) == hana::int_c<3>);
    static_assert(hana::pred(10) == 9, "");
}
