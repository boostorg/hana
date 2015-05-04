/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
using namespace boost::hana;


int main() {

{

//! [flatten]
BOOST_HANA_CONSTEXPR_CHECK(
    flatten(make<Tuple>(make<Tuple>(1, 2, 3), make<Tuple>(4, 5), make<Tuple>(6, 7, 8, 9)))
    ==
    make<Tuple>(1, 2, 3, 4, 5, 6, 7, 8, 9)
);

BOOST_HANA_CONSTANT_CHECK(flatten(nothing) == nothing);
BOOST_HANA_CONSTEXPR_CHECK(flatten(just(just(1))) == just(1));
BOOST_HANA_CONSTANT_CHECK(flatten(just(nothing)) == nothing);
//! [flatten]

}{

//! [monadic_compose]
BOOST_HANA_CONSTEXPR_LAMBDA auto block = [](auto ...types) {
    return [=](auto x) {
        return if_(contains(make_tuple(types...), decltype_(x)),
            nothing,
            just(x)
        );
    };
};

BOOST_HANA_CONSTEXPR_LAMBDA auto f = block(type<double>);
BOOST_HANA_CONSTEXPR_LAMBDA auto g = block(type<int>);
BOOST_HANA_CONSTEXPR_LAMBDA auto h = monadic_compose(g, f);
BOOST_HANA_CONSTANT_CHECK(h(1)    == nothing); // fails inside g; 1 has type int
BOOST_HANA_CONSTANT_CHECK(h(1.2)  == nothing); // fails inside f; 1.2 has type double
BOOST_HANA_CONSTEXPR_CHECK(h('x') == just('x')); // ok; 'x' has type char
//! [monadic_compose]

}{

//! [then]
struct undefined { };
BOOST_HANA_CONSTEXPR_CHECK(
    then(make<Tuple>(undefined{}, undefined{}), make<Tuple>(1, 2, 3)) == make<Tuple>(
        1, 2, 3,
        1, 2, 3
    )
);
//! [then]

}{

//! [tap]
std::stringstream before, after;

auto xs = make<Tuple>(1, 2, 3)
    | tap<Tuple>([&](auto x) { before << x << ' '; })
    | [](auto x) { return make<Tuple>(x, -x); }
    | tap<Tuple>([&](auto x) { after << x << ' '; });

BOOST_HANA_RUNTIME_CHECK(before.str() == "1 2 3 ");
BOOST_HANA_RUNTIME_CHECK(after.str() == "1 -1 2 -2 3 -3 ");
BOOST_HANA_RUNTIME_CHECK(xs == make<Tuple>(1, -1, 2, -2, 3, -3));
//! [tap]

}

}
