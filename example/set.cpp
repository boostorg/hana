/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {

//////////////////////////////////////////////////////////////////////////////
// Models
//////////////////////////////////////////////////////////////////////////////
{

//! [Comparable]
BOOST_HANA_CONSTANT_CHECK(
    set(int_<0>, type<char>, int_<1>) == set(int_<1>, int_<0>, type<char>)
);

BOOST_HANA_CONSTANT_CHECK(set(int_<0>, type<char>) != set(int_<1>));
//! [Comparable]

}{

//! [Searchable]
constexpr auto xs = set(int_<0>, int_<1>, int_<2>);
BOOST_HANA_CONSTANT_CHECK(find(xs, int_<0>) == just(int_<0>));
BOOST_HANA_CONSTANT_CHECK(find(xs, int_<3>) == nothing);

// operator[] is equivalent to at_key
BOOST_HANA_CONSTANT_CHECK(xs[int_<2>] == int_<2>);

// long_<0> == int_<0>, and therefore int_<0> is found
BOOST_HANA_CONSTANT_CHECK(xs[long_<0>] == int_<0>);
//! [Searchable]

}{

//! [Foldable]
constexpr auto xs = set(int_<0>, int_<1>, int_<2>);
static_assert(minimum(xs) == int_<0>, "");
static_assert(maximum(xs) == int_<2>, "");
static_assert(sum<>(xs) == int_<3>, "");
//! [Foldable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [make<Set>]
auto m = make<Set>(int_<1>, type<void>);
//! [make<Set>]
(void)m;

}{

//! [make_set]
BOOST_HANA_CONSTANT_CHECK(
    make_set(int_<1>, type<void>) == make<Set>(int_<1>, type<void>)
);
//! [make_set]

}{

//! [from_Foldable]
BOOST_HANA_CONSTANT_CHECK(
    to<Set>(make<Tuple>(int_<1>, int_<3>, type<int>, long_<1>))
        == make<Set>(int_<1>, int_<3>, type<int>)
);
//! [from_Foldable]

}{

//! [insert]
constexpr auto xs = set(int_<0>, type<int>);
BOOST_HANA_CONSTANT_CHECK(
    insert(xs, BOOST_HANA_STRING("abc")) ==
    set(int_<0>, type<int>, BOOST_HANA_STRING("abc"))
);

BOOST_HANA_CONSTANT_CHECK(
    insert(xs, int_<0>) == set(int_<0>, type<int>)
);
//! [insert]

}{

//! [erase_key]
constexpr auto xs = set(int_<0>, type<int>, type<void>);

BOOST_HANA_CONSTANT_CHECK(erase_key(xs, type<int>) == set(int_<0>, type<void>));
BOOST_HANA_CONSTANT_CHECK(erase_key(xs, type<char>) == xs);
//! [erase_key]

}

}
