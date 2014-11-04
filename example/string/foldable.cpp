/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/string.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    auto sum_string = [](auto str) {
        return foldl(str, int_<0>, [](auto sum, auto c) {
            constexpr int i = value(c) - 48; // convert character to decimal
            return sum + int_<i>;
        });
    };

    BOOST_HANA_CONSTANT_ASSERT(
        sum_string(BOOST_HANA_STRING("1234")) == int_<1 + 2 + 3 + 4>
    );
    //! [main]
}
