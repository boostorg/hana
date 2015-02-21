/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

//! [includes]
#include <boost/hana.hpp>
using namespace boost::hana;
//! [includes]


int main() {

//! [tuple_auto]
auto xs = make<Tuple>(1, '2', std::string{"345"});
//! [tuple_auto]

{

//! [tuple_type]
_tuple<int, char, std::string> xs{1, '2', "345"};
//! [tuple_type]

}{

//! [operations]
// BOOST_HANA_RUNTIME_CHECK is equivalent to assert
BOOST_HANA_RUNTIME_CHECK(last(xs) == "345");

BOOST_HANA_RUNTIME_CHECK(tail(xs) == make<Tuple>('2', std::string{"345"}));

static_assert(!is_empty(xs), "");

static_assert(length(xs) == 3u, "");

BOOST_HANA_RUNTIME_CHECK(at(int_<1>, xs) == '2');
//! [operations]

}{

//! [std_tuple_size]
std::tuple<int, char, std::string> xs{1, '2', std::string{"345"}};
static_assert(std::tuple_size<decltype(xs)>::value != 0, "");
//! [std_tuple_size]

}{

//! [transform]
// BOOST_HANA_CONSTEXPR_CHECK is equivalent to assert, but it would be
// equivalent to static_assert if lambdas could appear in constant expressions.
BOOST_HANA_CONSTEXPR_CHECK(
    transform(make<Tuple>(1, 2, 3), [](int i) { return i + 1; }) ==
    make<Tuple>(2, 3, 4)
);
//! [transform]

}{

//! [algorithms]
BOOST_HANA_CONSTEXPR_CHECK(
    count(make<Tuple>(1, 2, 3, 4, 5), [](int i) {
        return i > 2;
    }) == 3u
);

BOOST_HANA_CONSTEXPR_CHECK(
    filter(make<Tuple>(1, '2', 3.0, nullptr, 7.3f), [](auto x) {
        return std::is_floating_point<decltype(x)>{};
    }) == make<Tuple>(3.0, 7.3f)
);

using namespace std::literals; // the s user-defined literal creates std::strings
auto pangram = make<Tuple>(
    make<Tuple>("The"s, "quick"s, "brown"s),
    make<Tuple>("fox"s, "jumps"s, "over"s, "the"s, "lazy"s),
    make<Tuple>("dog"s)
);
BOOST_HANA_RUNTIME_CHECK(
    flatten(pangram) ==
    make<Tuple>("The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog")
);
//! [algorithms]

}

}
