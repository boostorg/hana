// Copyright Louis Dionne 2013-2016
// Copyright Jason Rice 2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/view.hpp>

#include <laws/base.hpp>
#include <support/seq.hpp>
namespace hana = boost::hana;
using hana::test::ct_eq;


int main() {
    auto container = ::seq;

    {
        const auto storage = container(
            container(ct_eq<0>{})
        );

        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<0>),
            container(ct_eq<0>{})
        ));
    }

    {
        const auto storage = container(
            container(ct_eq<0>{}),
            container(ct_eq<0>{})
        );

        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view, hana::size_c<0>),
            container(ct_eq<0>{}, ct_eq<0>{})
        ));
    }

    {
        const auto storage = container(
            container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
            container(ct_eq<3>{}, ct_eq<4>{})
        );

        auto view = hana::detail::cartesian_product_view(storage);

        auto expected = container(
            container(ct_eq<0>{}, ct_eq<3>{}),
            container(ct_eq<0>{}, ct_eq<4>{}),
            container(ct_eq<1>{}, ct_eq<3>{}),
            container(ct_eq<1>{}, ct_eq<4>{}),
            container(ct_eq<2>{}, ct_eq<3>{}),
            container(ct_eq<2>{}, ct_eq<4>{})
        );

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view,     hana::size_c<0>),
            hana::at(expected, hana::size_c<0>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view,     hana::size_c<1>),
            hana::at(expected, hana::size_c<1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view,     hana::size_c<2>),
            hana::at(expected, hana::size_c<2>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view,     hana::size_c<3>),
            hana::at(expected, hana::size_c<3>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view,     hana::size_c<4>),
            hana::at(expected, hana::size_c<4>)
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at(view,     hana::size_c<5>),
            hana::at(expected, hana::size_c<5>)
        ));
    }
}
