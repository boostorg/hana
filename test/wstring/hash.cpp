// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/hash.hpp>
#include <boost/hana/wstring.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::hash(hana::wstring_c<>),
        hana::type_c<hana::wstring<>>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::hash(hana::wstring_c<L'a'>),
        hana::type_c<hana::wstring<L'a'>>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::hash(hana::wstring_c<L'a', L'b'>),
        hana::type_c<hana::wstring<L'a', L'b'>>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::hash(hana::wstring_c<L'a', L'b', L'c'>),
        hana::type_c<hana::wstring<L'a', L'b', L'c'>>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::hash(hana::wstring_c<L'\xabcd', L'\xbcde', L'\xcdef'>),
        hana::type_c<hana::wstring<L'\xabcd', L'\xbcde', L'\xcdef'>>
    ));
}
