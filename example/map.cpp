/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/functional/placeholder.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/type.hpp>

#include <string>
using namespace boost::hana;


int main() {

//////////////////////////////////////////////////////////////////////////////
// Models
//////////////////////////////////////////////////////////////////////////////
{

//! [Comparable]
using namespace std::literals;

BOOST_HANA_RUNTIME_CHECK(
    make_map(
        make_pair(char_<'a'>, "foobar"s),
        make_pair(type<int&&>, nullptr)
    )
    ==
    make_map(
        make_pair(type<int&&>, (void*)0),
        make_pair(char_<'a'>, "foobar"s)
    )
);
//! [Comparable]

}{

//! [Searchable]
constexpr auto m = make_map(
    make_pair(type<int>, 'i'),
    make_pair(type<float>, 'f')
);
static_assert(find(m, type<int>) == just('i'), "");
static_assert(find(m, type<float>) == just('f'), "");
BOOST_HANA_CONSTANT_CHECK(find(m, type<void>) == nothing);
BOOST_HANA_CONSTANT_CHECK(find(m, int_<3>) == nothing);

// operator[] is equivalent to at_key
static_assert(m[type<int>] == 'i', "");
static_assert(m[type<float>] == 'f', "");
//! [Searchable]

}{

//! [Foldable]
using namespace std::literals;

// Given a map of (key, value) pairs, returns a map of (value, key) pairs.
// This requires both the keys and the values to be compile-time Comparable.
auto invert = [](auto map) {
    return fold.left(map, make_map(), [](auto map, auto pair) {
        return insert(map, make_pair(second(pair), first(pair)));
    });
};

auto m = make_map(
    make_pair(type<int>, int_<1>),
    make_pair(type<float>, int_<2>),
    make_pair(int_<3>, type<void>)
);

BOOST_HANA_CONSTANT_CHECK(invert(m) ==
    make_map(
        make_pair(int_<1>, type<int>),
        make_pair(int_<2>, type<float>),
        make_pair(type<void>, int_<3>)
    )
);
//! [Foldable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [make<Map>]
using namespace std::literals;

auto m = make<Map>(
    make<Pair>(int_<1>, "foobar"s),
    make<Pair>(type<void>, 1234)
);

BOOST_HANA_CONSTANT_CHECK(is_a<Map>(m));
//! [make<Map>]

}{

//! [make_map]
using namespace std::literals;

BOOST_HANA_RUNTIME_CHECK(
    make_map(
        make<Pair>(int_<1>, "foobar"s),
        make<Pair>(type<void>, 1234)
    )
    ==
    make<Map>(
        make<Pair>(int_<1>, "foobar"s),
        make<Pair>(type<void>, 1234)
    )
);
//! [make_map]

}{

//! [values]
using namespace std::literals;

auto m = make<Map>(
    make<Pair>(int_<1>, "foobar"s),
    make<Pair>(type<void>, 1234)
);

// The order of the values is unspecified.
BOOST_HANA_RUNTIME_CHECK(values(m) ^in^ permutations(make<Tuple>("foobar"s, 1234)));
//! [values]

}{

//! [keys]
using namespace std::literals;

auto m = make<Map>(
    make<Pair>(int_<1>, "foobar"s),
    make<Pair>(type<void>, 1234)
);

// The order of the keys is unspecified.
BOOST_HANA_CONSTANT_CHECK(keys(m) ^in^ permutations(make<Tuple>(int_<1>, type<void>)));
//! [keys]

}{

//! [from_Foldable]
using namespace std::literals;

BOOST_HANA_RUNTIME_CHECK(
    to<Map>(make<Tuple>(
        make<Pair>(type<int>, "abcd"s),
        make<Pair>(type<void>, 1234),
        make<Pair>(type<int>, nullptr)
    )) == make<Map>(
        make<Pair>(type<int>, "abcd"s),
        make<Pair>(type<void>, 1234)
    )
);
//! [from_Foldable]

}{

//! [insert]
using namespace std::literals;

auto m = make_map(
    make_pair(type<int>, "abcd"s),
    make_pair(type<void>, 1234)
);

BOOST_HANA_RUNTIME_CHECK(
    insert(m, make_pair(type<float>, 'x')) ==
    make_map(
        make_pair(type<int>, "abcd"s),
        make_pair(type<void>, 1234),
        make_pair(type<float>, 'x')
    )
);

BOOST_HANA_RUNTIME_CHECK(insert(m, make_pair(type<void>, 'x')) == m);
//! [insert]

}{

//! [erase_key]
using namespace std::literals;

auto m = make_map(
    make_pair(type<int>, "abcd"s),
    make_pair(type<void>, 1234),
    make_pair(BOOST_HANA_STRING("foobar!"), type<char>)
);

BOOST_HANA_RUNTIME_CHECK(
    erase_key(m, BOOST_HANA_STRING("foobar!")) ==
    make_map(
        make_pair(type<int>, "abcd"s),
        make_pair(type<void>, 1234)
    )
);

BOOST_HANA_RUNTIME_CHECK(erase_key(m, type<char>) == m);
//! [erase_key]

}

}
