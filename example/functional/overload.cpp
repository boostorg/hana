/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

//! [main]
#include <boost/hana/assert.hpp>
#include <boost/hana/functional/overload.hpp>

#include <iostream>
#include <string>
using namespace boost::hana;


int on_int(int i) {
    std::cout << "matched int: " << i << std::endl;
    return i;
}

int main() {
    auto on_string = [](std::string const& s) {
        std::cout << "matched std::string: " << s << std::endl;
        return s;
    };

    auto f = overload(on_int, on_string);

    // prints "matched int: 1"
    BOOST_HANA_CHECK(f(1) == 1);

    // prints "matched std::string: abcdef"
    BOOST_HANA_CHECK(f("abcdef") == std::string{"abcdef"});
}
//! [main]
