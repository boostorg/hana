/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>


struct f {
    template <typename State, typename X>
    constexpr X operator()(State, X x) const { return x; }
};

struct state { };

template <int i>
struct x { };

int main() {
    constexpr auto tuple = boost::hana::tuple_c<unsigned long long,
        <%= (1..input_size).to_a.join(', ') %>
    >;
    constexpr auto result = boost::hana::foldl(tuple, state{}, f{});
    (void)result;
}
