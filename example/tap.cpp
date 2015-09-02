/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/tap.hpp>
#include <boost/hana/tuple.hpp>

#include <sstream>
namespace hana = boost::hana;


int main() {
    std::stringstream before, after;

    auto xs = hana::make_tuple(1, 2, 3)
        | hana::tap<hana::tuple_tag>([&](auto x) { before << x << ' '; })
        | [](auto x) { return hana::make_tuple(x, -x); }
        | hana::tap<hana::tuple_tag>([&](auto x) { after << x << ' '; });

    BOOST_HANA_RUNTIME_CHECK(before.str() == "1 2 3 ");
    BOOST_HANA_RUNTIME_CHECK(after.str() == "1 -1 2 -2 3 -3 ");
    BOOST_HANA_RUNTIME_CHECK(xs == hana::make_tuple(1, -1, 2, -2, 3, -3));
}
