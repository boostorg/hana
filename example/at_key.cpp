/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/at_key.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>

#include <string>
namespace hana = boost::hana;


int main() {
    auto m = hana::make_map(
        hana::make_pair(hana::type_c<int>, std::string{"int"}),
        hana::make_pair(hana::int_c<3>, std::string{"3"})
    );

    BOOST_HANA_RUNTIME_CHECK(hana::at_key(m, hana::type_c<int>) == "int");

    // usage as operator[]
    BOOST_HANA_RUNTIME_CHECK(m[hana::type_c<int>] == "int");
    BOOST_HANA_RUNTIME_CHECK(m[hana::int_c<3>] == "3");
}
