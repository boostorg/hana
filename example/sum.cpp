/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/sum.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(hana::sum<>(hana::make_range(hana::int_<1>, hana::int_<6>)) == hana::int_<15>);

static_assert(hana::sum<>(hana::make_tuple(1, hana::int_<3>, hana::long_<-5>, 9)) == 8, "");

static_assert(hana::sum<unsigned long>(hana::make_tuple(1ul, 3ul)) == 4ul, "");

int main() { }
