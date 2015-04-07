/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <string>
#include <type_traits>
using namespace boost::hana;


int main() {

{

//! [concat]
using namespace literals;
BOOST_HANA_CONSTEXPR_CHECK(
    concat(make<Tuple>(1, '2'), make<Tuple>(3.3, 4_c)) == make<Tuple>(1, '2', 3.3, 4_c)
);
//! [concat]

}{

//! [empty]
BOOST_HANA_CONSTANT_CHECK(empty<Tuple>() == make<Tuple>());
BOOST_HANA_CONSTANT_CHECK(empty<Maybe>() == nothing);
//! [empty]

}{

//! [prepend]
BOOST_HANA_CONSTEXPR_CHECK(prepend(1, make<Tuple>()) == make<Tuple>(1));
BOOST_HANA_CONSTEXPR_CHECK(prepend(1, make<Tuple>('2', 3.3)) == make<Tuple>(1, '2', 3.3));
BOOST_HANA_CONSTEXPR_CHECK(
    prepend(1, prepend('2', prepend(3.3, make<Tuple>()))) == make<Tuple>(1, '2', 3.3)
);
//! [prepend]

}{

//! [append]
BOOST_HANA_CONSTEXPR_CHECK(append(make<Tuple>(), 1) == make<Tuple>(1));
BOOST_HANA_CONSTEXPR_CHECK(append(make<Tuple>(1, '2'), 3.3) == make<Tuple>(1, '2', 3.3));
BOOST_HANA_CONSTEXPR_CHECK(
    append(append(append(make<Tuple>(), 1), '2'), 3.3) == make<Tuple>(1, '2', 3.3)
);
//! [append]

}{

//! [filter]
static_assert(filter(make<Tuple>(1, 2.0, 3, 4.0), trait<std::is_integral>) == make<Tuple>(1, 3), "");
static_assert(filter(just(3), trait<std::is_integral>) == just(3), "");
BOOST_HANA_CONSTANT_CHECK(filter(just(3.0), trait<std::is_integral>) == nothing);
//! [filter]

}{

//! [cycle]
static_assert(cycle(size_t<2>, make<Tuple>('x', 'y')) == make<Tuple>('x', 'y', 'x', 'y'), "");
//! [cycle]

}{

//! [remove_if]
static_assert(remove_if(make<Tuple>(1, 2.0, 3, 4.0), trait<std::is_integral>) == make<Tuple>(2.0, 4.0), "");
static_assert(remove_if(just(3.0), trait<std::is_integral>) == just(3.0), "");
BOOST_HANA_CONSTANT_CHECK(remove_if(just(3), trait<std::is_integral>) == nothing);
//! [remove_if]

}{

using boost::hana::remove; // Make sure we don't clash with ::remove from <stdio.h>
//! [remove]
BOOST_HANA_CONSTANT_CHECK(remove(tuple_t<int, char, float>, type<char>) == tuple_t<int, float>);
BOOST_HANA_CONSTANT_CHECK(remove(just(type<int>), type<char>) == just(type<int>));
BOOST_HANA_CONSTANT_CHECK(remove(just(type<int>), type<int>) == nothing);
//! [remove]

}{

//! [repeat]
BOOST_HANA_CONSTEXPR_CHECK(repeat<Tuple>(size_t<2>, 'x') == make<Tuple>('x', 'x'));

// Of course, there can't be more than one element in a Maybe.
static_assert(repeat<Maybe>(size_t<2>, 'x') == just('x'), "");
//! [repeat]

}{

//! [prefix]
using namespace std::literals;
BOOST_HANA_RUNTIME_CHECK(
    prefix("my"s, make<Tuple>("dog"s, "car"s, "house"s)) ==
    make<Tuple>("my", "dog", "my", "car", "my", "house")
);
//! [prefix]

}{

//! [suffix]
BOOST_HANA_CONSTEXPR_CHECK(
    suffix(0, make<Tuple>(1, 2, 3, 4)) == make<Tuple>(1, 0, 2, 0, 3, 0, 4, 0)
);
//! [suffix]

}

}
