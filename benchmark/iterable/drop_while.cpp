/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool/logical.hpp>
#include <boost/hana/iterable/iterable.hpp>

#include "benchmark.hpp"

<%= setup %>

template <int i> struct x { };


int main() {
    auto iterable = <%= iterable %>;
    auto pred = [](auto&& x) { return boost::hana::true_; };

    boost::hana::benchmark::measure([=] {
        boost::hana::drop_while(iterable, pred);
    });
}
