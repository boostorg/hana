/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

//////////////////////////////////////////////////////////////////////////////
// Important: Keep this file in sync with the Hello World in the README
//////////////////////////////////////////////////////////////////////////////

#include <boost/hana.hpp>
#include <iostream>
#include <string>
using namespace boost::hana;

int main() {
    auto xs = list("Hell", int_<0>, std::string{" world"}, '!');

    // > "Hell0 world!"
    for_each(xs, [](auto x) { std::cout << x; });

    // > "Hello world!"
    for_each(replace(_ == int_<0>, 'o', xs), [](auto x) { std::cout << x; });
}
