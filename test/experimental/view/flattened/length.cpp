// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/experimental/view.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>

#include <laws/base.hpp>
#include <support/seq.hpp>
namespace hana = boost::hana;
using hana::test::ct_eq;


int main() {
    auto container = ::seq;

    {
        auto storage1 = container();
        auto storage = container(storage1);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<0>
        ));
    }{
        auto storage1 = container();
        auto storage2 = container();
        auto storage = container(storage1, storage2);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<0>
        ));
    }{
        auto storage1 = container();
        auto storage2 = container();
        auto storage3 = container();
        auto storage = container(storage1, storage2, storage3);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<0>
        ));
    }

    {
        auto storage1 = container(ct_eq<0>{});
        auto storage2 = container();
        auto storage = container(storage1, storage2);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<1>
        ));
    }{
        auto storage1 = container();
        auto storage2 = container(ct_eq<0>{});
        auto storage = container(storage1, storage2);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<1>
        ));
    }

    {
        auto storage1 = container(ct_eq<0>{}, ct_eq<1>{});
        auto storage2 = container();
        auto storage = container(storage1, storage2);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<2>
        ));
    }{
        auto storage1 = container(ct_eq<0>{});
        auto storage2 = container(ct_eq<1>{});
        auto storage = container(storage1, storage2);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<2>
        ));
    }{
        auto storage1 = container();
        auto storage2 = container(ct_eq<0>{}, ct_eq<1>{});
        auto storage = container(storage1, storage2);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<2>
        ));
    }

    {
        auto storage1 = container(ct_eq<0>{}, ct_eq<1>{});
        auto storage2 = container(ct_eq<2>{}, ct_eq<3>{});
        auto storage = container(storage1, storage2);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<4>
        ));
    }

    {
        auto storage1 = container(ct_eq<0>{}, ct_eq<1>{});
        auto storage2 = container(ct_eq<2>{}, ct_eq<3>{});
        auto storage3 = container(ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{});
        auto storage = container(storage1, storage2, storage3);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<7>
        ));
    }

    {
        auto storage1 = container(ct_eq<0>{}, ct_eq<1>{});
        auto storage2 = container();
        auto storage3 = container(ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{});
        auto storage4 = container();
        auto storage = container(storage1, storage2, storage3, storage4);
        auto flat = hana::experimental::flattened(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(flat),
            hana::size_c<5>
        ));
    }
}
