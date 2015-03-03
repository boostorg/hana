/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <sstream>
using namespace boost::hana;


//! [bind]
// sfinae_impl(f, x...) is just(f(x...)) if that expression is
// well-formed, and nothing otherwise.
template <typename F, typename ...X>
constexpr auto sfinae_impl(F f, X ...x) -> decltype(just(f(x...)))
{ return just(f(x...)); }

constexpr auto sfinae_impl(...) { return nothing; }

// sfinae(f)(x...) is equivalent to sfinae_impl(f, x...). In other words,
// sfinae(f)(x...) is just(f(x...)) if that expression is well-formed,
// and nothing otherwise.
template <typename F>
BOOST_HANA_CONSTEXPR_LAMBDA auto sfinae(F f) {
    return [=](auto ...x) { return sfinae_impl(f, x...); };
}

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
