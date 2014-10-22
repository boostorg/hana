/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/fwd/list.hpp>

#include "benchmark.hpp"

<%= setup %>

template <int i> struct x { };


int main() {
    auto list = <%= list %>;
    struct state { };
    auto f = [](auto&& s, auto&& x) -> decltype(auto) {
        return boost::hana::detail::std::forward<decltype(x)>(x);
    };

    boost::hana::benchmark::measure([=] {
        boost::hana::scanl(list, state{}, f);
    });
}
