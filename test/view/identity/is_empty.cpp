// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/view.hpp>

#include <laws/base.hpp>
#include <support/seq.hpp>
namespace hana = boost::hana;
using hana::test::ct_eq;


int main() {
    auto container = ::seq;

    {
        auto storage = container();
        auto view = hana::experimental::identity_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(view));
    }

    {
        auto storage = container(ct_eq<0>{});
        auto view = hana::experimental::identity_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(view)));
    }

    {
        auto storage = container(ct_eq<0>{}, ct_eq<1>{});
        auto view = hana::experimental::identity_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(view)));
    }

    {
        auto storage = container(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{});
        auto view = hana::experimental::identity_view(storage);
        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(view)));
    }
}
