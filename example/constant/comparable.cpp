/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


//! [main]
struct Person {
    int age;
    // ...
};

int main() {
    BOOST_HANA_STATIC_ASSERT(integral<int, 1> == integral<long, 1>);
    BOOST_HANA_STATIC_ASSERT(integral<int, 1> != integral<int, 2>);
    BOOST_HANA_STATIC_ASSERT(integral<int, 1> != integral<int Person::*, &Person::age>);
}
//! [main]
