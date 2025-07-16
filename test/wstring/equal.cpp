// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/not_equal.hpp> // for operator !=
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    // equal
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        BOOST_HANA_AUTO_STRING(L"abcd")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_AUTO_STRING(L"\xabcd\xef01"),
        BOOST_HANA_AUTO_STRING(L"\xabcd\xef01")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        BOOST_HANA_AUTO_STRING(L"abcde")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        BOOST_HANA_AUTO_STRING(L"")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_AUTO_STRING(L"\xaa00"),
        BOOST_HANA_AUTO_STRING(L"")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        BOOST_HANA_AUTO_STRING(L""),
        BOOST_HANA_AUTO_STRING(L"abcde")
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        BOOST_HANA_AUTO_STRING(L""),
        BOOST_HANA_AUTO_STRING(L"")
    ));

    // check operators
    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_AUTO_STRING(L"abcd")
            ==
        BOOST_HANA_AUTO_STRING(L"abcd")
    );

    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_AUTO_STRING(L"\xabcd")
            ==
        BOOST_HANA_AUTO_STRING(L"\xabcd")
    );

    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_AUTO_STRING(L"abcd")
            !=
        BOOST_HANA_AUTO_STRING(L"abc")
    );

    BOOST_HANA_CONSTANT_CHECK(
        BOOST_HANA_AUTO_STRING(L"\xabcd")
            !=
        BOOST_HANA_AUTO_STRING(L"\xabc0")
    );
}
