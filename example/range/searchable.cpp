/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/range.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(hana::find(hana::make_range(hana::int_<1>, hana::int_<25>), hana::int_<10>) == hana::just(hana::int_<10>));
BOOST_HANA_CONSTANT_CHECK(hana::find(hana::make_range(hana::int_<1>, hana::int_<25>), hana::int_<200>) == hana::nothing);

int main() { }
