/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/fwd/functor.hpp>

#include "benchmark.hpp"

<%= setup %>


int main() {
    auto f = [](auto&& x) -> decltype(auto) {
        return boost::hana::detail::std::forward<decltype(x)>(x);
    };
    auto pred = [](auto&& x) { return boost::hana::true_; };
    auto functor = <%= functor %>;

    boost::hana::benchmark::measure([=] {
        boost::hana::adjust_if(functor, pred, f);
    });
}
