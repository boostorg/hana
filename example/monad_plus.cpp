/*
@copyright Louis Dionne 2014
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
    concat(tuple(1, '2'), tuple(3.3, 4_c)) == tuple(1, '2', 3.3, 4_c)
);
//! [concat]

}{

//! [nil]
BOOST_HANA_CONSTANT_CHECK(nil<Tuple>() == tuple());
BOOST_HANA_CONSTANT_CHECK(nil<Maybe>() == nothing);
//! [nil]

}{

//! [prepend]
BOOST_HANA_CONSTEXPR_CHECK(prepend(1, tuple()) == tuple(1));
BOOST_HANA_CONSTEXPR_CHECK(prepend(1, tuple('2', 3.3)) == tuple(1, '2', 3.3));
BOOST_HANA_CONSTEXPR_CHECK(
    prepend(1, prepend('2', prepend(3.3, tuple()))) == tuple(1, '2', 3.3)
);
//! [prepend]

}{

//! [append]
BOOST_HANA_CONSTEXPR_CHECK(append(tuple(), 1) == tuple(1));
BOOST_HANA_CONSTEXPR_CHECK(append(tuple(1, '2'), 3.3) == tuple(1, '2', 3.3));
BOOST_HANA_CONSTEXPR_CHECK(
    append(append(append(tuple(), 1), '2'), 3.3) == tuple(1, '2', 3.3)
);
//! [append]

}{

//! [filter]
BOOST_HANA_CONSTEXPR_CHECK(
    filter(tuple(1, 2.0, 3, 4.0), trait_<std::is_integral>) == tuple(1, 3)
);

BOOST_HANA_CONSTEXPR_CHECK(
    filter(just(3), trait_<std::is_integral>) == just(3)
);

BOOST_HANA_CONSTANT_CHECK(
    filter(just(3.0), trait_<std::is_integral>) == nothing
);
//! [filter]

}{

//! [cycle]
BOOST_HANA_CONSTEXPR_CHECK(
    cycle(size_t<2>, tuple('x', 'y', 'z')) == tuple('x', 'y', 'z', 'x', 'y', 'z')
);
//! [cycle]

}{

//! [repeat]
BOOST_HANA_CONSTEXPR_CHECK(repeat<Tuple>(size_t<2>, 'x') == tuple('x', 'x'));

// Of course, because Maybe can hold at most one element.
static_assert(repeat<Maybe>(size_t<2>, 'x') == just('x'), "");
//! [repeat]

}{

//! [prefix]
using namespace std::literals;
BOOST_HANA_RUNTIME_CHECK(
    prefix("my"s, tuple("dog"s, "car"s, "house"s)) ==
    tuple("my", "dog", "my", "car", "my", "house")
);
//! [prefix]

}{

//! [suffix]
BOOST_HANA_CONSTEXPR_CHECK(
    suffix(0, tuple(1, 2, 3, 4)) == tuple(1, 0, 2, 0, 3, 0, 4, 0)
);
//! [suffix]

}

}
