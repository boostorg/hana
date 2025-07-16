// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTANT_CHECK(hana::is_empty(BOOST_HANA_AUTO_STRING(L"")));
    BOOST_HANA_CONSTANT_CHECK(hana::is_empty(hana::wstring_c<>));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(BOOST_HANA_AUTO_STRING(L"a"))));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(BOOST_HANA_AUTO_STRING(L"ab"))));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(BOOST_HANA_AUTO_STRING(L"abc"))));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(hana::wstring_c<L'a'>)));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(hana::wstring_c<L'\xa000'>)));
}
