// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/greater.hpp>
#include <boost/hana/greater_equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/less_equal.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        BOOST_HANA_AUTO_STRING(L""),
        BOOST_HANA_AUTO_STRING(L"")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_AUTO_STRING(L""),
        BOOST_HANA_AUTO_STRING(L"a")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        BOOST_HANA_AUTO_STRING(L"a"),
        BOOST_HANA_AUTO_STRING(L"")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_AUTO_STRING(L"a"),
        BOOST_HANA_AUTO_STRING(L"ab")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        BOOST_HANA_AUTO_STRING(L"ab"),
        BOOST_HANA_AUTO_STRING(L"ab")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_AUTO_STRING(L"abc"),
        BOOST_HANA_AUTO_STRING(L"abcde")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_AUTO_STRING(L"abcde"),
        BOOST_HANA_AUTO_STRING(L"abfde")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        BOOST_HANA_AUTO_STRING(L"\xabcd"),
        BOOST_HANA_AUTO_STRING(L"\xabcd\xabcd")
    ));

    // check operators
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_AUTO_STRING(L"abc") < BOOST_HANA_AUTO_STRING(L"abcd")
    );
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_AUTO_STRING(L"abc") <= BOOST_HANA_AUTO_STRING(L"abcd")
    );
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_AUTO_STRING(L"abcd") > BOOST_HANA_AUTO_STRING(L"abc")
    );
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_AUTO_STRING(L"abcd") >= BOOST_HANA_AUTO_STRING(L"abc")
    );
}
