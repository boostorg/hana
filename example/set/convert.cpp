/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


int main() {
    constexpr auto xs = hana::make_tuple(hana::int_<1>, hana::int_<3>, hana::type_c<int>, hana::long_<1>);
    BOOST_HANA_CONSTANT_CHECK(
        hana::to<hana::Set>(xs) == hana::make_set(hana::int_<1>, hana::int_<3>, hana::type_c<int>)
    );
}