// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/drop_front_exactly.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_AUTO_STRING(L"a")),
        BOOST_HANA_AUTO_STRING(L"")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_AUTO_STRING(L"ab")),
        BOOST_HANA_AUTO_STRING(L"b")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_AUTO_STRING(L"abc")),
        BOOST_HANA_AUTO_STRING(L"bc")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_AUTO_STRING(L"abcdefghijk")),
        BOOST_HANA_AUTO_STRING(L"bcdefghijk")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_AUTO_STRING(L"\xabcd" L"efg")),
        BOOST_HANA_AUTO_STRING(L"efg")
    ));


    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_AUTO_STRING(L"abc"), hana::size_c<2>),
        BOOST_HANA_AUTO_STRING(L"c")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_AUTO_STRING(L"abcdefghijk"), hana::size_c<3>),
        BOOST_HANA_AUTO_STRING(L"defghijk")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::drop_front_exactly(BOOST_HANA_AUTO_STRING(L"a\xbcde\xf012" L"abc"), hana::size_c<2>),
        BOOST_HANA_AUTO_STRING(L"\xf012" L"abc")
    ));

}
