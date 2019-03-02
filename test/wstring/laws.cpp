// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/wstring.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/hashable.hpp>
#include <laws/iterable.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <laws/searchable.hpp>
namespace hana = boost::hana;


int main() {
    // Comparable and Hashable
    {
        auto strings = hana::make_tuple(
            BOOST_HANA_AUTO_STRING(L""),
            BOOST_HANA_AUTO_STRING(L"a"),
            BOOST_HANA_AUTO_STRING(L"ab"),
            BOOST_HANA_AUTO_STRING(L"abc\xdef0"),
            BOOST_HANA_AUTO_STRING(L"abcd\xef01"),
            BOOST_HANA_AUTO_STRING(L"abcde\xf012"),
            BOOST_HANA_AUTO_STRING(L"ba\xbcde")
        );

        hana::test::TestComparable<hana::wstring_tag>{strings};
        hana::test::TestHashable<hana::wstring_tag>{strings};
    }

    // Monoid
    {
        auto strings = hana::make_tuple(
            BOOST_HANA_AUTO_STRING(L""),
            BOOST_HANA_AUTO_STRING(L"a"),
            BOOST_HANA_AUTO_STRING(L"ab"),
            BOOST_HANA_AUTO_STRING(L"abc\xdef0"),
            BOOST_HANA_AUTO_STRING(L"abcd\xef01"),
            BOOST_HANA_AUTO_STRING(L"abcde\xf012"),
            BOOST_HANA_AUTO_STRING(L"ba\xbcde")
        );

        hana::test::TestMonoid<hana::wstring_tag>{strings};
    }

    // Foldable and Iterable
    {
        auto strings = hana::make_tuple(
            BOOST_HANA_AUTO_STRING(L""),
            BOOST_HANA_AUTO_STRING(L"a"),
            BOOST_HANA_AUTO_STRING(L"ab"),
            BOOST_HANA_AUTO_STRING(L"abc\xdef0"),
            BOOST_HANA_AUTO_STRING(L"abcd\xef01"),
            BOOST_HANA_AUTO_STRING(L"abcde\xf012"),
            BOOST_HANA_AUTO_STRING(L"ba\xbcde"),
            BOOST_HANA_AUTO_STRING(L"afcd\xef01")
        );

        hana::test::TestFoldable<hana::wstring_tag>{strings};
        hana::test::TestIterable<hana::wstring_tag>{strings};
    }

    // Orderable
    {
        auto strings = hana::make_tuple(
            BOOST_HANA_AUTO_STRING(L""),
            BOOST_HANA_AUTO_STRING(L"a"),
            BOOST_HANA_AUTO_STRING(L"ab"),
            BOOST_HANA_AUTO_STRING(L"abc\xdef0"),
            BOOST_HANA_AUTO_STRING(L"ba\xbcde"),
            BOOST_HANA_AUTO_STRING(L"abd\xef01")
        );

        hana::test::TestOrderable<hana::wstring_tag>{strings};
    }

    // Searchable
    {
        auto keys = hana::tuple_c<wchar_t, L'a', L'f'>;
        auto strings = hana::make_tuple(
            BOOST_HANA_AUTO_STRING(L""),
            BOOST_HANA_AUTO_STRING(L"a"),
            BOOST_HANA_AUTO_STRING(L"ab"),
            BOOST_HANA_AUTO_STRING(L"abcd\xef01"),
            BOOST_HANA_AUTO_STRING(L"ba\xbcde"),
            BOOST_HANA_AUTO_STRING(L"afcd")
        );

        hana::test::TestSearchable<hana::wstring_tag>{strings, keys};
    }
}
