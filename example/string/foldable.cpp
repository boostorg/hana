/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/value.hpp>
namespace hana = boost::hana;


int main() {
    auto sum_string = [](auto str) {
        return hana::fold_left(str, hana::int_<0>, [](auto sum, auto c) {
            constexpr int i = hana::value(c) - 48; // convert character to decimal
            return sum + hana::int_<i>;
        });
    };

    BOOST_HANA_CONSTANT_CHECK(
        sum_string(BOOST_HANA_STRING("1234")) == hana::int_<1 + 2 + 3 + 4>
    );
}
