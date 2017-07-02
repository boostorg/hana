// Copyright Louis Dionne 2013-2016
// Copyright Jason Rice 2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/unpack.hpp>
#include <boost/hana/view.hpp>

#include <laws/base.hpp>
#include <support/seq.hpp>
namespace hana = boost::hana;
using hana::test::ct_eq;


int main() {
    auto container = ::seq;
    auto make_container = [](auto ...x) {
        return ::seq(x...);
    };

    {
        auto storage = container();
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(view));
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(hana::unpack(view, make_container)));
    }

    {
        auto storage = container(container());
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(view));
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(hana::unpack(view, make_container)));
    }

    {
        auto storage = container(container(), container(), container());
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(view));
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(hana::unpack(view, make_container)));
    }

    {
        auto storage = container(container(ct_eq<0>{}), container());
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(view));
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(hana::unpack(view, make_container)));
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
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::unpack(view, make_container),
            container()
        ));
    }

    {
        auto storage = container(container(ct_eq<0>{}));
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::unpack(view, make_container),
            container(container(ct_eq<0>{}))
        ));
    }

    {
        auto storage = container(
            container(ct_eq<0>{}),
            container(ct_eq<1>{}),
            container(ct_eq<2>{}, ct_eq<3>{}),
            container(ct_eq<4>{})
        );
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::unpack(view, make_container),
            container(
              container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<4>{}),
              container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<3>{}, ct_eq<4>{})
            )
        ));
    }

    {
        auto storage = container(
            container(ct_eq<0>{}),
            container(ct_eq<1>{}),
            container(ct_eq<2>{}, ct_eq<3>{}),
            container(ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{})
        );
        auto view = hana::detail::cartesian_product_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::unpack(view, make_container),
            container(
              container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<4>{}),
              container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<5>{}),
              container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<6>{}),
              container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<3>{}, ct_eq<4>{}),
              container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<3>{}, ct_eq<5>{}),
              container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<3>{}, ct_eq<6>{})
            )
        ));
    }
}
