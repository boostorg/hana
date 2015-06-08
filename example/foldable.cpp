/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;


int main() {

using boost::hana::size_t; // disambiguate with ::size_t on GCC

{

//! [conversion]
static_assert(to<Tuple>(just(1)) == make<Tuple>(1), "");
BOOST_HANA_CONSTANT_CHECK(to<Tuple>(nothing) == make<Tuple>());

BOOST_HANA_CONSTANT_CHECK(to<Tuple>(make_range(int_<3>, int_<6>)) == tuple_c<int, 3, 4, 5>);
//! [conversion]

}{

//! [fold.left]
auto to_string = [](auto x) {
    std::ostringstream ss;
    ss << x;
    return ss.str();
};

auto f = [=](std::string s, auto element) {
    return "f(" + s + ", " + to_string(element) + ")";
};

// with an initial state
BOOST_HANA_RUNTIME_CHECK(
    fold.left(make<Tuple>(2, '3', 4, 5.0), "1", f)
        ==
    "f(f(f(f(1, 2), 3), 4), 5)"
);

// without initial state
BOOST_HANA_RUNTIME_CHECK(
    fold.left(make<Tuple>("1", 2, '3', 4, 5.0), f)
        ==
    "f(f(f(f(1, 2), 3), 4), 5)"
);
//! [fold.left]

}{

//! [fold.right]
auto to_string = [](auto x) {
    std::ostringstream ss;
    ss << x;
    return ss.str();
};

auto f = [=](auto element, std::string s) {
    return "f(" + to_string(element) + ", " + s + ")";
};

// with an initial state
BOOST_HANA_RUNTIME_CHECK(
    fold.right(make<Tuple>(1, '2', 3.0, 4), "5", f)
        ==
    "f(1, f(2, f(3, f(4, 5))))"
);

// without initial state
BOOST_HANA_RUNTIME_CHECK(
    fold.right(make<Tuple>(1, '2', 3.0, 4, "5"), f)
        ==
    "f(1, f(2, f(3, f(4, 5))))"
);
//! [fold.right]

}{

//! [monadic_fold.right]
BOOST_HANA_CONSTEXPR_LAMBDA auto safediv = [](auto x, auto y) {
    return eval_if(y == int_<0>,
        always(nothing),
        [=](auto _) { return just(_(x) / y); }
    );
};

// with an initial state
BOOST_HANA_CONSTANT_CHECK(
    monadic_fold<Optional>.right(tuple_c<int, 1000, 8, 4>, int_<2>, safediv)
        ==
    just(int_<1000> / (int_<8> / (int_<4> / int_<2>)))
);

BOOST_HANA_CONSTANT_CHECK(
    monadic_fold<Optional>.right(tuple_c<int, 1000, 8, 4>, int_<0>, safediv)
        ==
    nothing
);

// without an initial state
BOOST_HANA_CONSTANT_CHECK(
    monadic_fold<Optional>.right(tuple_c<int, 1000, 8, 4, 2>, safediv)
        ==
    just(int_<1000> / (int_<8> / (int_<4> / int_<2>)))
);

BOOST_HANA_CONSTANT_CHECK(
    monadic_fold<Optional>.right(tuple_c<int, 1000, 8, 4, 0>, safediv)
        ==
    nothing
);
//! [monadic_fold.right]

}{

// [reverse_fold]
auto to_string = [](auto x) {
    std::ostringstream ss;
    ss << x;
    return ss.str();
};

auto f = [=](std::string s, auto element) {
    return "f(" + s + ", " + to_string(element) + ")";
};

// With an initial state
BOOST_HANA_RUNTIME_CHECK(
    reverse_fold(make<Tuple>(1, '2', 3.0, 4), "5", f)
        ==
    "f(f(f(f(5, 4), 3), 2), 1)"
);

// Without an initial state
BOOST_HANA_RUNTIME_CHECK(
    reverse_fold(make<Tuple>(1, '2', 3.0, 4, "5"), f)
        ==
    "f(f(f(f(5, 4), 3), 2), 1)"
);
// [reverse_fold]

}{

//! [for_each]
std::stringstream ss;
for_each(make<Tuple>(0, '1', "234", 5.5), [&](auto x) {
    ss << x << ' ';
});

BOOST_HANA_RUNTIME_CHECK(ss.str() == "0 1 234 5.5 ");
//! [for_each]

}{

//! [length]
BOOST_HANA_CONSTANT_CHECK(length(make<Tuple>()) == size_t<0>);
BOOST_HANA_CONSTANT_CHECK(length(make<Tuple>(1, '2', 3.0)) == size_t<3>);

BOOST_HANA_CONSTANT_CHECK(length(nothing) == size_t<0>);
BOOST_HANA_CONSTANT_CHECK(length(just('x')) == size_t<1>);
//! [length]

}{

//! [size]
BOOST_HANA_CONSTANT_CHECK(size(make<Tuple>()) == size_t<0>);
BOOST_HANA_CONSTANT_CHECK(size(make<Tuple>(1, '2', 3.0)) == size_t<3>);

BOOST_HANA_CONSTANT_CHECK(size(nothing) == size_t<0>);
BOOST_HANA_CONSTANT_CHECK(size(just('x')) == size_t<1>);
//! [size]

}{

//! [product]
BOOST_HANA_CONSTANT_CHECK(
    product<>(range(int_<1>, int_<6>)) == int_<1 * 2 * 3 * 4 * 5>
);

BOOST_HANA_CONSTEXPR_CHECK(
    product<>(make<Tuple>(1, int_<3>, long_<-5>, 9)) == 1 * 3 * -5 * 9
);

BOOST_HANA_CONSTEXPR_CHECK(
    product<unsigned long>(make<Tuple>(2ul, 3ul)) == 6ul
);
//! [product]

}{

//! [sum]
BOOST_HANA_CONSTANT_CHECK(
    sum<>(range(int_<1>, int_<6>)) == int_<1 + 2 + 3 + 4 + 5>
);

BOOST_HANA_CONSTEXPR_CHECK(
    sum<>(make<Tuple>(1, int_<3>, long_<-5>, 9)) == 1 + 3 - 5 + 9
);

BOOST_HANA_CONSTEXPR_CHECK(
    sum<unsigned long>(make<Tuple>(1ul, 3ul)) == 4ul
);
//! [sum]

}{

//! [unpack]
BOOST_HANA_CONSTEXPR_LAMBDA auto add = [](auto x, auto y, auto z) {
    return x + y + z;
};

BOOST_HANA_CONSTEXPR_CHECK(unpack(make_tuple(1, 2, 3), add) == 6);
//! [unpack]

}{

//! [fuse]
auto cheap_tie = [](auto& ...vars) {
    return fuse([&vars...](auto ...values) {
        // Using an initializer list sequences the assignments.
        int dummy[] = {0, ((void)(vars = values), 0)...};
        (void)dummy;
    });
};
int a = 0;
char b = '\0';
double c = 0;

cheap_tie(a, b, c)(make<Tuple>(1, '2', 3.3));
BOOST_HANA_RUNTIME_CHECK(a == 1 && b == '2' && c == 3.3);
//! [fuse]

}{

//! [count_if]
using namespace literals;
BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % 2_c != 0_c;
};

constexpr auto types = tuple_t<int, char, long, short, char, double>;
constexpr auto ints = tuple_c<int, 1, 2, 3>;

BOOST_HANA_CONSTANT_CHECK(count_if(ints, odd) == size_t<2>);

BOOST_HANA_CONSTANT_CHECK(count_if(types, trait<std::is_floating_point>) == size_t<1>);
BOOST_HANA_CONSTANT_CHECK(count_if(types, _ == type<char>) == size_t<2>);
BOOST_HANA_CONSTANT_CHECK(count_if(types, _ == type<void>) == size_t<0>);
//! [count_if]

}{

//! [count]
constexpr auto types = tuple_t<int, char, long, short, char, double>;
constexpr auto ints = tuple_c<int, 1, 2, 3, 2, 2, 4, 2>;

BOOST_HANA_CONSTANT_CHECK(count(ints, int_<2>) == size_t<4>);
BOOST_HANA_CONSTEXPR_CHECK(count(ints, 2) == 4);
BOOST_HANA_CONSTANT_CHECK(count(types, type<char>) == size_t<2>);
//! [count]

}{

//! [maximum]
// without a predicate
BOOST_HANA_CONSTANT_CHECK(
    maximum(tuple_c<int, -1, 0, 2, -4, 6, 9>) == int_<9>
);

// with a predicate
BOOST_HANA_CONSTANT_CHECK(
    maximum(tuple_c<int, -1, 0, 2, -4, 6, 9>, [](auto x, auto y) {
        return x > y;
    }) == int_<-4>
);
//! [maximum]

}{

//! [maximum.by]
BOOST_HANA_CONSTEXPR_CHECK(
    maximum.by(ordering(length), make_tuple(
        make_tuple(),
        make_tuple(1, '2'),
        make_tuple(3.3, nullptr, 4)
    ))
    == make_tuple(3.3, nullptr, 4)
);
//! [maximum.by]

}{

//! [minimum]
// without a predicate
BOOST_HANA_CONSTANT_CHECK(
    minimum(tuple_c<int, -1, 0, 2, -4, 6, 9>) == int_<-4>
);

// with a predicate
BOOST_HANA_CONSTANT_CHECK(
    minimum(tuple_c<int, -1, 0, 2, -4, 6, 9>, [](auto x, auto y) {
        return x > y;
    }) == int_<9>
);
//! [minimum]

}{

//! [minimum.by]
BOOST_HANA_CONSTANT_CHECK(
    minimum.by(ordering(length), make_tuple(
        make_tuple(),
        make_tuple(1, '2'),
        make_tuple(3.3, nullptr, 4)
    ))
    == make_tuple()
);
//! [minimum.by]

}

}
