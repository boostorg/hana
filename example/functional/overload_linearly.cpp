/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/overload_linearly.hpp>

#include <string>
using namespace boost::hana;


int main() {
//! [main]
auto f = overload_linearly(
    [](int i) { return i + 1; },
    [](std::string s) { return s + "d"; },
    [](double d) { throw "never called"; }
);

BOOST_HANA_RUNTIME_ASSERT(f(1) == 2);
BOOST_HANA_RUNTIME_ASSERT(f("abc") == "abcd");
BOOST_HANA_RUNTIME_ASSERT(f(2.2) == static_cast<int>(2.2) + 1);
//! [main]
}
