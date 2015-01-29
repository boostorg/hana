/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
using namespace boost::hana;


namespace bind_example {

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
constexpr decltype(auto) sfinae(F f)
{ return partial(perhaps, f); }

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

void main() {
    Person john{30};

    // Can't dereference a non-pointer.
    BOOST_HANA_CONSTANT_CHECK(f(john) == nothing);

    // `int` has no member named `age`.
    BOOST_HANA_CONSTANT_CHECK(f(1) == nothing);

    // All is good.
    BOOST_HANA_CONSTEXPR_CHECK(f(&john) == just(30u));
}
//! [bind]

}


int main() {
    bind_example::main();

{

//! [flatten]
BOOST_HANA_CONSTEXPR_CHECK(
    flatten(tuple(tuple(1, 2, 3), tuple(4, 5), tuple(6, 7, 8, 9)))
    ==
    tuple(1, 2, 3, 4, 5, 6, 7, 8, 9)
);

BOOST_HANA_CONSTANT_CHECK(flatten(nothing) == nothing);
BOOST_HANA_CONSTEXPR_CHECK(flatten(just(just(1))) == just(1));
BOOST_HANA_CONSTANT_CHECK(flatten(just(nothing)) == nothing);
//! [flatten]

}{

//! [mcompose]
BOOST_HANA_CONSTEXPR_LAMBDA auto block = [](auto ...types) {
    return [=](auto x) {
        return if_(elem(tuple(types...), decltype_(x)),
            nothing,
            just(x)
        );
    };
}

BOOST_HANA_CONSTEXPR_LAMBDA auto f = block(type<double>);
BOOST_HANA_CONSTEXPR_LAMBDA auto g = block(type<int>);
BOOST_HANA_CONSTEXPR_LAMBDA auto h = mcompose<Maybe>(f, g);
BOOST_HANA_CONSTANT_CHECK(h(1)    == nothing);   // fails inside g; 1 has type int
BOOST_HANA_CONSTANT_CHECK(h(1.2)  == nothing); // fails inside f; 1.2 has type double
BOOST_HANA_CONSTEXPR_CHECK(h('x') == just('x')); // ok; 'x' has type char
//! [mcompose]

}{

//! [then]
struct undefined { };
BOOST_HANA_CONSTEXPR_CHECK(
    then(tuple(undefined{}, undefined{}), tuple(1, 2, 3)) == tuple(
        1, 2, 3,
        1, 2, 3
    )
);
//! [then]

}{

//! [tap]
std::stringstream before, after;

auto xs = tuple(1, 2, 3)
    | tap<Tuple>([&](auto x) { before << x << ' '; })
    | [](auto x) { return tuple(x, -x); }
    | tap<Tuple>([&](auto x) { after << x << ' '; });

BOOST_HANA_RUNTIME_CHECK(before.str() == "1 2 3 ");
BOOST_HANA_RUNTIME_CHECK(after.str() == "1 -1 2 -2 3 -3 ");
BOOST_HANA_RUNTIME_CHECK(xs == tuple(1, -1, 2, -2, 3, -3));
//! [tap]

}

}
