// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/any_of.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::any_of(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        hana::equal.to(hana::wchar_c<L'b'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::any_of(
        BOOST_HANA_AUTO_STRING(L"\xabcd\xef12\x3456"),
        hana::equal.to(hana::wchar_c<L'\xef12'>)
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::any_of(
        BOOST_HANA_AUTO_STRING(L""),
        hana::always(hana::true_c)
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::any_of(
        BOOST_HANA_AUTO_STRING(L"abcd"),
        hana::equal.to(hana::wchar_c<L'z'>)
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::any_of(
        BOOST_HANA_AUTO_STRING(L"\xabcd\xef12\x3456"),
        hana::equal.to(hana::wchar_c<L'\xcd'>)
    )));
}
