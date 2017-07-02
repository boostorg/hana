// Copyright Louis Dionne 2013-2016
// Copyright Jason Rice 2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/view.hpp>

#include <laws/base.hpp>
#include <support/seq.hpp>
namespace hana = boost::hana;
using hana::test::ct_eq;


int main() {
    auto container = ::seq;

    {
        auto storage = container(container());
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(view));
    }

    {
        auto storage = container(container(ct_eq<0>{}), container());
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(view));
    }

    {
        auto storage = container(
            container(ct_eq<0>{}),
            container(ct_eq<1>{}),
            container(ct_eq<2>{}, ct_eq<3>{}),
            container(),
            container(ct_eq<4>{})
        );
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(view));
    }

    {
        auto storage = container(container(ct_eq<0>{}));
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(view)));
    }

    {
        auto storage = container(
            container(ct_eq<0>{}),
            container(ct_eq<1>{}),
            container(ct_eq<2>{}, ct_eq<3>{}),
            container(ct_eq<4>{})
        );
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(view)));
    }

    {
        auto storage = container(
            container(ct_eq<0>{}),
            container(ct_eq<1>{}),
            container(ct_eq<2>{}, ct_eq<3>{}),
            container(ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{})
        );
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(view)));
    }
}
