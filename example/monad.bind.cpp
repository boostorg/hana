/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


//! [bind]
BOOST_HANA_CONSTEXPR_LAMBDA auto deref = [](auto x) -> decltype(*x) {
    return *x;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto age = [](auto x) -> decltype(x.age) {
    return x.age;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return bind(sfinae(deref)(x), sfinae(age));
};

struct Person {
    unsigned int age;
    // ...
};

int main() {
    Person john{30};

    // Can't dereference a non-pointer.
    BOOST_HANA_CONSTANT_CHECK(f(john) == nothing);

    // `int` has no member named `age`.
    BOOST_HANA_CONSTANT_CHECK(f(1) == nothing);

    // All is good.
    BOOST_HANA_CONSTEXPR_CHECK(f(&john) == just(30u));
}
//! [bind]
