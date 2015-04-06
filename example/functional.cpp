/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <iostream>
#include <string>
using namespace boost::hana;


int main() {


{

//! [always]
static_assert(always(1)() == 1, "");
static_assert(always('2')(1, 2, 3) == '2', "");
//! [always]

}{

//! [apply]
static_assert(apply(plus, 1, 2) == 3, "");
//! [apply]

}{

//! [arg]
// arg<0>(1, '2', 3.3); // static assertion (regardless of the number of arguments)
BOOST_HANA_CONSTEXPR_CHECK(arg<1>(1, '2', 3.3) == 1);
BOOST_HANA_CONSTEXPR_CHECK(arg<2>(1, '2', 3.3) == '2');
BOOST_HANA_CONSTEXPR_CHECK(arg<3>(1, '2', 3.3) == 3.3);
// arg<4>(1, '2', 3.3); // static assertion
//! [arg]

}{

//! [capture]
BOOST_HANA_CONSTEXPR_LAMBDA auto sum = [](auto x, auto y, auto z) {
    return x + y + z;
};

BOOST_HANA_CONSTEXPR_CHECK(capture(1, 2, 3)(sum)() == 6);
BOOST_HANA_CONSTEXPR_CHECK(capture(1, 2)(sum)(3) == 6);
//! [capture]

}{

//! [compose]
BOOST_HANA_CONSTEXPR_LAMBDA auto to_char = [](int x) {
    return static_cast<char>(x + 48);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto increment = [](auto x) {
    return x + 1;
};

BOOST_HANA_CONSTEXPR_CHECK(compose(to_char, increment)(3) == '4');
//! [compose]

}{

//! [curry]
BOOST_HANA_CONSTEXPR_LAMBDA auto add = [](auto x, auto y, auto z) {
    return x + y + z;
};

BOOST_HANA_CONSTEXPR_CHECK(curry<3>(add)(1)(2)(3) == 1 + 2 + 3);
BOOST_HANA_CONSTEXPR_CHECK(curry<3>(add)(1)(2, 3) == curry<3>(add)(1)(2)(3));
BOOST_HANA_CONSTEXPR_CHECK(curry<3>(add)(1, 2, 3) == curry<3>(add)(1)(2)(3));
//! [curry]


//! [curry0]
BOOST_HANA_CONSTEXPR_LAMBDA auto two = []() {
    return 2;
};

BOOST_HANA_CONSTEXPR_CHECK(curry<0>(two)() == two());
//! [curry0]

}{

//! [demux]
constexpr auto f = demux(make<Tuple>)(
    _ + _,
    _ - _,
    _ * _,
    _ / _
);

static_assert(
    f(10, 4) == make<Tuple>(
        10 + 4,
        10 - 4,
        10 * 4,
        10 / 4
    )
, "");
//! [demux]

}{

//! [fix]
BOOST_HANA_CONSTEXPR_LAMBDA auto factorial = fix([](auto fact, auto n) {
    if (n == 0) return 1;
    else        return n * fact(n - 1);
});

BOOST_HANA_CONSTEXPR_CHECK(factorial(5) == 120);
//! [fix]

}{

//! [flip]
BOOST_HANA_CONSTEXPR_LAMBDA auto minus = [](int x, int y, int z = 0) {
    return x - y - z;
};

BOOST_HANA_CONSTEXPR_CHECK(minus(3, 0) == 3 - 0);
BOOST_HANA_CONSTEXPR_CHECK(flip(minus)(3, 0) == 0 - 3);

BOOST_HANA_CONSTEXPR_CHECK(minus(3, 0, 1) == 3 - 0 - 1);
BOOST_HANA_CONSTEXPR_CHECK(flip(minus)(3, 0, 1) == 0 - 3 - 1);
//! [flip]

}{

//! [id]
static_assert(id(1) == 1, "");
static_assert(id('x') == 'x', "");
//! [id]

}{

//! [infix]
constexpr auto is_subset_of = infix(subset);
static_assert(make<Tuple>(1, '2', 3.3) ^is_subset_of^ make<Tuple>('2', 1, 3.3, "xyz"), "");

constexpr auto contains = infix(elem);
BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, '2', 3.3) ^contains^ 3.3);
//! [infix]

}{

//! [iterate]
constexpr auto next_10 = iterate<10>(succ);
static_assert(next_10(3) == 13, "");

constexpr auto xs = make<Tuple>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
static_assert(iterate<5>(tail, xs) == make<Tuple>(6, 7, 8, 9, 10), "");
//! [iterate]

}{

//! [on.infix]
constexpr auto sorted = sort.by(less ^on^ first, make<Tuple>(
    pair(int_<3>, 'x'),
    pair(int_<1>, type<void>),
    pair(int_<2>, 9876)
));

static_assert(sorted == make<Tuple>(
    pair(int_<1>, type<void>),
    pair(int_<2>, 9876),
    pair(int_<3>, 'x')
), "");
//! [on.infix]

}{

//! [on.normal]
BOOST_HANA_CONSTEXPR_LAMBDA auto increment = [](auto x) {
    return x + 1;
};
BOOST_HANA_CONSTEXPR_CHECK(on(mult, increment)(2, 4) == 3 * 5);

constexpr auto x = make_pair(1, 2);
constexpr auto y = make_pair(10, 20);
static_assert(on(plus, first)(x, y) == 1 + 10, "");
//! [on.normal]

}{

//! [lockstep]
BOOST_HANA_CONSTEXPR_LAMBDA auto to_int = [](char c) {
    return static_cast<int>(c) - 48;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) {
    return x + 1;
};

BOOST_HANA_CONSTEXPR_CHECK(lockstep(plus)(to_int, inc)('3', 4) == 3 + 5);
//! [lockstep]

}{

//! [overload]
auto on_string = [](std::string const& s) {
    std::cout << "matched std::string: " << s << std::endl;
    return s;
};

auto on_int = [](int i) {
    std::cout << "matched int: " << i << std::endl;
    return i;
};

auto f = overload(on_int, on_string);

// prints "matched int: 1"
BOOST_HANA_CHECK(f(1) == 1);

// prints "matched std::string: abcdef"
BOOST_HANA_CHECK(f("abcdef") == std::string{"abcdef"});
//! [overload]

}{

//! [overload_linearly]
auto f = overload_linearly(
    [](int i) { return i + 1; },
    [](std::string s) { return s + "d"; },
    [](double d) { throw "never called"; }
);

BOOST_HANA_RUNTIME_ASSERT(f(1) == 2);
BOOST_HANA_RUNTIME_ASSERT(f("abc") == "abcd");
BOOST_HANA_RUNTIME_ASSERT(f(2.2) == static_cast<int>(2.2) + 1);
//! [overload_linearly]

}{

//! [partial]
constexpr auto increment = partial(plus, 1);
static_assert(increment(2) == 3, "");
//! [partial]

}{

//! [placeholder]
BOOST_HANA_CONSTEXPR_LAMBDA auto plus = _ + _;
BOOST_HANA_CONSTEXPR_CHECK(plus(1, 2) == 1 + 2);

BOOST_HANA_CONSTEXPR_LAMBDA auto increment = _ + 1;
BOOST_HANA_CONSTEXPR_CHECK(increment(1) == 2);

BOOST_HANA_CONSTEXPR_LAMBDA auto double_ = 2 * _;
BOOST_HANA_CONSTEXPR_CHECK(double_(1) == 2);

// Extra arguments are ignored.
BOOST_HANA_CONSTEXPR_CHECK(double_(1, "ignored") == 2);
//! [placeholder]

}

}
