/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable/foldable.hpp>

#include <boost/hana/benchmark/measure.hpp>

<%= setup %>

template <int i> struct x { };


int main() {
    struct state { };
    auto f = [](auto&& s, auto&& x) -> decltype(auto) {
        return boost::hana::detail::std::forward<decltype(x)>(x);
    };
    auto foldable = <%= foldable %>;

    boost::hana::benchmark::measure([=] {
        boost::hana::foldl(foldable, state{}, f);
    });
}
