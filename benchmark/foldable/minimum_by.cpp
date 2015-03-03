/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/fwd/foldable.hpp>

#include "benchmark.hpp"

<%= setup %>


int main() {
    auto foldable = <%= foldable %>;
    auto pred = [](auto&& x, auto&& y) { return boost::hana::true_; };

    boost::hana::benchmark::measure([=] {
        boost::hana::minimum_by(pred, foldable);
    });
}
