/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    //! [comparable]
    struct Person {
        int age;
        // ...
    };

    BOOST_HANA_CONSTANT_ASSERT(integral<int, 1> == integral<long, 1>);
    BOOST_HANA_CONSTANT_ASSERT(integral<int, 1> != integral<int, 2>);
    BOOST_HANA_CONSTANT_ASSERT(integral<int, 1> != integral<int Person::*, &Person::age>);
    //! [comparable]

    //! [value]
    auto i = integral<int, 3>; // notice no constexpr
    static_assert(value(i) == 3, "value(i) is always a constant expression!");
    //! [value]
}

