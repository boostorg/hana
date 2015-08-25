/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(
        hana::make_set(hana::int_<0>, hana::type<char>, hana::int_<1>)
            ==
        hana::make_set(hana::int_<1>, hana::int_<0>, hana::type<char>)
    );

    BOOST_HANA_CONSTANT_CHECK(
        hana::make_set(hana::int_<0>, hana::type<char>)
            !=
        hana::make_set(hana::int_<1>)
    );
}
