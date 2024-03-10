// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


struct invalid { };

int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_AUTO_STRING(L"abcd"), hana::wchar_c<L'a'>),
        hana::just(hana::wchar_c<L'a'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_AUTO_STRING(L"abcd"), hana::wchar_c<L'c'>),
        hana::just(hana::wchar_c<L'c'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_AUTO_STRING(L"\x9876" L"abcd"), hana::wchar_c<L'\x9876'>),
        hana::just(hana::wchar_c<L'\x9876'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_AUTO_STRING(L"\x9876" L"abcd"), hana::wchar_c<L'c'>),
        hana::just(hana::wchar_c<L'c'>)
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_AUTO_STRING(L"abcd"), invalid{}),
        hana::nothing
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::find(BOOST_HANA_AUTO_STRING(L"\xabcd"), invalid{}),
        hana::nothing
    ));
}
