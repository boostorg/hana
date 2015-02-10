/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/string.hpp>
using namespace boost::hana;


int main() {

{

//! [BOOST_HANA_STRING]
BOOST_HANA_CONSTEXPR_LAMBDA auto str = BOOST_HANA_STRING("abcdef");
BOOST_HANA_CONSTANT_CHECK(is_a<String>(str));
//! [BOOST_HANA_STRING]

}{

//! [string]
constexpr auto str = string<'a', 'b', 'c', 'd', 'e', 'f'>;
BOOST_HANA_CONSTANT_CHECK(is_a<String>(str));
//! [string]

}{

//! [comparable]
BOOST_HANA_CONSTANT_CHECK(
    BOOST_HANA_STRING("abcdef") == BOOST_HANA_STRING("abcdef")
);

BOOST_HANA_CONSTANT_CHECK(
    BOOST_HANA_STRING("abcdef") != BOOST_HANA_STRING("abef")
);
//! [comparable]

}{

//! [orderable]
BOOST_HANA_CONSTANT_CHECK(
    BOOST_HANA_STRING("abc") < BOOST_HANA_STRING("bcd")
);

BOOST_HANA_CONSTANT_CHECK(
    BOOST_HANA_STRING("abcd") > BOOST_HANA_STRING("abc")
);
//! [orderable]

}{

//! [foldable]
auto sum_string = [](auto str) {
    return foldl(str, int_<0>, [](auto sum, auto c) {
        constexpr int i = value(c) - 48; // convert character to decimal
        return sum + int_<i>;
    });
};

BOOST_HANA_CONSTANT_CHECK(
    sum_string(BOOST_HANA_STRING("1234")) == int_<1 + 2 + 3 + 4>
);
//! [foldable]

}{

//! [iterable]
BOOST_HANA_CONSTANT_CHECK(!is_empty(BOOST_HANA_STRING("abcd")));
BOOST_HANA_CONSTANT_CHECK(is_empty(BOOST_HANA_STRING("")));

BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("abcd")[int_<2>] == char_<'c'>);

auto is_vowel = [](auto c) {
    return c ^in^ BOOST_HANA_STRING("aeiouy");
};
BOOST_HANA_CONSTANT_CHECK(
    drop_while(BOOST_HANA_STRING("aioubcd"), is_vowel) == BOOST_HANA_STRING("bcd")
);
//! [iterable]

}{

//! [searchable]
BOOST_HANA_CONSTANT_CHECK(char_<'c'> ^in^ BOOST_HANA_STRING("abcde"));
BOOST_HANA_CONSTANT_CHECK(!(char_<'z'> ^in^ BOOST_HANA_STRING("abcde")));

BOOST_HANA_CONSTANT_CHECK(
    lookup(BOOST_HANA_STRING("abcxefg"), char_<'x'>) == just(char_<'x'>)
);
//! [searchable]

}

}
