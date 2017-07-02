// Copyright Louis Dionne 2013-2016
// Copyright Jason Rice 2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/view.hpp>

#include <support/seq.hpp>
namespace hana = boost::hana;


int main() {
    auto container = ::seq;

    auto storage = container(container(0),
                             container(1),
                             container(2, 3),
                             container(4, 5, 6));

    auto view = hana::detail::cartesian_product_view(storage);

    hana::at_c<0>(hana::at_c<0>(view)) = 90;
    hana::at_c<1>(hana::at_c<0>(view)) = 91;
    hana::at_c<2>(hana::at_c<0>(view)) = 92;
    hana::at_c<3>(hana::at_c<0>(view)) = 94;
    hana::at_c<3>(hana::at_c<1>(view)) = 95;
    hana::at_c<2>(hana::at_c<5>(view)) = 93;
    hana::at_c<3>(hana::at_c<5>(view)) = 96;

    BOOST_HANA_RUNTIME_CHECK(hana::equal(
        storage,
        container(container(90),
                  container(91),
                  container(92, 93),
                  container(94, 95, 96))
    ));
}
