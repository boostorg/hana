/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/string.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {

//////////////////////////////////////////////////////////////////////////////
// Modeled concepts
//////////////////////////////////////////////////////////////////////////////
{

//! [Comparable]
BOOST_HANA_CONSTANT_CHECK(
    BOOST_HANA_STRING("abcdef") == BOOST_HANA_STRING("abcdef")
);

BOOST_HANA_CONSTANT_CHECK(
    BOOST_HANA_STRING("abcdef") != BOOST_HANA_STRING("abef")
);
//! [Comparable]

}{

//! [Orderable]
BOOST_HANA_CONSTANT_CHECK(
    BOOST_HANA_STRING("abc") < BOOST_HANA_STRING("bcd")
);

BOOST_HANA_CONSTANT_CHECK(
    BOOST_HANA_STRING("abcd") > BOOST_HANA_STRING("abc")
);
//! [Orderable]

}{

//! [Foldable]
auto sum_string = [](auto str) {
    return fold.left(str, int_<0>, [](auto sum, auto c) {
        constexpr int i = value(c) - 48; // convert character to decimal
        return sum + int_<i>;
    });
};

BOOST_HANA_CONSTANT_CHECK(
    sum_string(BOOST_HANA_STRING("1234")) == int_<1 + 2 + 3 + 4>
);
//! [Foldable]

}{

//! [Iterable]
BOOST_HANA_CONSTANT_CHECK(!is_empty(BOOST_HANA_STRING("abcd")));
BOOST_HANA_CONSTANT_CHECK(is_empty(BOOST_HANA_STRING("")));

BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("abcd")[int_<2>] == char_<'c'>);

auto is_vowel = [](auto c) {
    return c ^in^ BOOST_HANA_STRING("aeiouy");
};
BOOST_HANA_CONSTANT_CHECK(
    drop_while(BOOST_HANA_STRING("aioubcd"), is_vowel) == BOOST_HANA_STRING("bcd")
);
//! [Iterable]

}{

//! [Searchable]
BOOST_HANA_CONSTANT_CHECK(char_<'c'> ^in^ BOOST_HANA_STRING("abcde"));
BOOST_HANA_CONSTANT_CHECK(!(char_<'z'> ^in^ BOOST_HANA_STRING("abcde")));

BOOST_HANA_CONSTANT_CHECK(
    find(BOOST_HANA_STRING("abcxefg"), char_<'x'>) == just(char_<'x'>)
);
//! [Searchable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [BOOST_HANA_STRING]
BOOST_HANA_CONSTEXPR_LAMBDA auto str = BOOST_HANA_STRING("abcdef");
BOOST_HANA_CONSTANT_CHECK(str == string<'a', 'b', 'c', 'd', 'e', 'f'>);
static_assert(std::is_same<datatype_t<decltype(str)>, String>{}, "");
//! [BOOST_HANA_STRING]

}{

//! [string]
constexpr auto str = string<'a', 'b', 'c', 'd', 'e', 'f'>;
static_assert(std::is_same<datatype_t<decltype(str)>, String>{}, "");
//! [string]

}{

//! [to<char const*>]
constexpr auto str = string<'h', 'i'>;
constexpr char const* s = to<char const*>(str);
static_assert(s[0] == 'h' && s[1] == 'i' && s[2] == '\0', "");
//! [to<char const*>]

}

}
