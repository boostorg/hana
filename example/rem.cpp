/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/rem.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::rem(hana::int_<6>, hana::int_<4>) == hana::int_<2>);
    BOOST_HANA_CONSTANT_CHECK(hana::rem(hana::int_<-6>, hana::int_<4>) == hana::int_<-2>);
    static_assert(hana::rem(6, 4) == 2, "");
}
