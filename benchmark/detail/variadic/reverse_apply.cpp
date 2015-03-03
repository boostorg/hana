/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include "benchmark.hpp"

<%= setup %>

template <int i> struct x { };


int main() {
    auto f = [](auto&& ...x) { };

    boost::hana::benchmark::measure([=] {
        boost::hana::detail::variadic::<%= reverse_apply %>(f,
            <%= (1..input_size).map{ |i| "x<#{i}>{}" }.join(', ') %>
        );
    });
}
