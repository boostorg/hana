// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/view.hpp>

#include <support/seq.hpp>
namespace hana = boost::hana;


int main() {
    auto container = ::seq;

    auto storage = container(0, 1, 2);
    auto view = hana::detail::identity_view(storage);
    hana::at_c<0>(view) = 90;
    hana::at_c<1>(view) = 91;
    hana::at_c<2>(view) = 92;

    BOOST_HANA_RUNTIME_CHECK(hana::at_c<0>(view) == 90);
    BOOST_HANA_RUNTIME_CHECK(hana::at_c<1>(view) == 91);
    BOOST_HANA_RUNTIME_CHECK(hana::at_c<2>(view) == 92);

    BOOST_HANA_RUNTIME_CHECK(hana::at_c<0>(storage) == 90);
    BOOST_HANA_RUNTIME_CHECK(hana::at_c<1>(storage) == 91);
    BOOST_HANA_RUNTIME_CHECK(hana::at_c<2>(storage) == 92);
}
