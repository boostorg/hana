/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/fwd/iterable.hpp>
#include <boost/hana/integral_constant.hpp>

#include "benchmark.hpp"

<%= setup %>

template <int i> struct x { };


int main() {
    auto iterable = <%= iterable %>;
    auto n = boost::hana::int_c< <%= input_size / 2 %> >;

    boost::hana::benchmark::measure([=] {
        boost::hana::drop(n, iterable);
    });
}
