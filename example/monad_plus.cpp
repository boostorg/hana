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
BOOST_HANA_CONSTEXPR_CHECK(
    filter(make<Tuple>(1, 2.0, 3, 4.0), trait_<std::is_integral>) == make<Tuple>(1, 3)
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
    cycle(size_t<2>, make<Tuple>('x', 'y', 'z')) == make<Tuple>('x', 'y', 'z', 'x', 'y', 'z')
);
//! [cycle]

}{

//! [repeat]
BOOST_HANA_CONSTEXPR_CHECK(repeat<Tuple>(size_t<2>, 'x') == make<Tuple>('x', 'x'));

// Of course, because Maybe can hold at most one element.
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
