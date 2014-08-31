/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable/iterable.hpp>

#include <boost/hana/benchmark/measure.hpp>

<%= setup %>


int main() {
    auto iterable = <%= iterable %>;

    boost::hana::benchmark::measure([=] {
        boost::hana::last(iterable);
    });
}
