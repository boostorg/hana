// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/view.hpp>

#include <laws/base.hpp>
#include <support/seq.hpp>
namespace hana = boost::hana;
using hana::test::ct_eq;


int main() {
    auto container = ::seq;

    {
        auto storage = container(ct_eq<0>{});
        auto view = hana::detail::identity_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<0>),
            ct_eq<0>{}
        ));
    }

    {
        auto storage = container(ct_eq<0>{}, ct_eq<1>{});
        auto view = hana::detail::identity_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<0>),
            ct_eq<0>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<1>),
            ct_eq<1>{}
        ));
    }

    {
        auto storage = container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{});
        auto view = hana::detail::identity_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<0>),
            ct_eq<0>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<1>),
            ct_eq<1>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<2>),
            ct_eq<2>{}
        ));
    }

    {
        auto storage = container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{});
        auto view = hana::detail::identity_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<0>),
            ct_eq<0>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<1>),
            ct_eq<1>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<2>),
            ct_eq<2>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<3>),
            ct_eq<3>{}
        ));
    }
}
