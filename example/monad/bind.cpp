/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/sandbox/detail/is_valid.hpp>
using namespace boost::hana;


// `sfinae(f)(x...)` returns `just(f(x...))` if `f(x...)` is well-formed,
// and `nothing` otherwise.
BOOST_HANA_CONSTEXPR_LAMBDA auto sfinae = [](auto f) {
    return [=](auto ...x) {
        return eval_if(detail::is_valid(f)(x...),
            [=](auto _) { return just(_(f)(x...)); },
            [](auto _) { return nothing; }
        );
    };
};

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
    BOOST_HANA_STATIC_ASSERT(f(john) == nothing);

    // `int` has no member named `age`.
    BOOST_HANA_STATIC_ASSERT(f(1) == nothing);

    // All is good.
    BOOST_HANA_STATIC_ASSERT(f(&john) == just(30u));
}
