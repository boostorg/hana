/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/unpack.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTANT_CHECK(
    hana::fold_left(hana::make_range(hana::int_<0>, hana::int_<4>), hana::int_<0>, hana::plus) == hana::int_<6>
);

BOOST_HANA_CONSTANT_CHECK(
    hana::unpack(hana::make_range(hana::int_<-2>, hana::int_<2>), hana::make_tuple) ==
    hana::make_tuple(hana::int_<-2>, hana::int_<-1>, hana::int_<0>, hana::int_<1>)
);

int main() { }
