/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


//! [main]
struct Person {
    int age;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(constant<int, 1> == constant<long, 1>);
    BOOST_HANA_STATIC_ASSERT(constant<int, 1> != constant<int, 2>);
    BOOST_HANA_STATIC_ASSERT(constant<int, 1> != constant<int Person::*, &Person::age>);
}
//! [main]
