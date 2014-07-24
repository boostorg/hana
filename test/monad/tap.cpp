/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monad/bind_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/monad.hpp>

#include <cassert>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);
struct invalid { };

template <template <typename ...> class mcd>
void test() {
    constexpr auto monad = detail::minimal::monad<mcd>;
    using M = detail::minimal::Monad<mcd>;

    bool executed = false;
    auto m = bind(monad(x<0>), tap<M>([&](auto) { executed = true; }));
    assert(m == monad(x<0>));
    assert(executed);
}

int main() {
    test<Monad::flatten_mcd>();
    test<Monad::bind_mcd>();
}
