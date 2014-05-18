/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename T> struct wrap { };
template <typename T> void f(wrap<T>) { }

template <bool b = false>
struct invalid { static_assert(b, "invalid must not be instantiated"); };

struct incomplete;

int main() {
    // ADL kicks in and invalid<> is instantiated
    // f(wrap<invalid<>>{});

    // ADL kicks in but incomplete can't be instantiated
    // f(wrap<incomplete>{});

    // ADL instantiates the types recursively
    // f(wrap<wrap<invalid<>>>{});

    // ADL is disabled by (...)
    // (f)(wrap<invalid<>>{});
}
