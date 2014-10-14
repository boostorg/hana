/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/overload.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;
using sandbox::overload;


auto g(char) { return type<char>; }
struct otherwise { };

int main() {
    auto f = overload(
        [](int) { return type<int>; },
        [](float) { return type<float>; },
        g,
        [](auto) { return type<otherwise>; }
    );
    BOOST_HANA_CONSTANT_ASSERT(equal(f(int{}), type<int>));
    BOOST_HANA_CONSTANT_ASSERT(equal(f(float{}), type<float>));
    BOOST_HANA_CONSTANT_ASSERT(equal(f(char{}), type<char>));
    BOOST_HANA_CONSTANT_ASSERT(equal(f(otherwise{}), type<otherwise>));
}
