// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/view.hpp>
namespace hana = boost::hana;


int main() {
    int x = 3;
    auto view = hana::detail::single_view(x);
    hana::at_c<0>(view) = 93;

    BOOST_HANA_RUNTIME_CHECK(hana::at_c<0>(view) == 93);
    BOOST_HANA_RUNTIME_CHECK(x == 93);
}
