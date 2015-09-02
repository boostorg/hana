/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/fwd/sequence.hpp>

#include "benchmark.hpp"

<%= setup %>

template <int i> struct x { };

struct pred {
    template <int i>
    constexpr decltype(auto) operator()(x<i> const&) const {
        return boost::hana::bool_c<i ==  <%= input_size / 2 %>   >;
    }
};

int main() {
    using L = <%= datatype %>;
    auto list = boost::hana::make<L>(
        <%= (1..input_size).to_a.map { |i| "x<#{i}>{}" }.join(', ') %>
    );

    boost::hana::benchmark::measure([=] {
        boost::hana::take_while(list, pred{});
    });
}
