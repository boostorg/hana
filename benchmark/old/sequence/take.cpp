/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/fwd/sequence.hpp>

#include "benchmark.hpp"

<%= setup %>

template <int i> struct x { };


int main() {
    using L = <%= datatype %>;
    auto list = boost::hana::make<L>(
        <%= (1..input_size).to_a.map { |i| "x<#{i}>{}" }.join(', ') %>
    );

    auto n = boost::hana::int_c< <%= input_size / 2 %> >;

    boost::hana::benchmark::measure([=] {
        boost::hana::take(n, list);
    });
}
