/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

template <bool b = false>
struct invalid { static_assert(b, "invalid must not be instantiated"); };

struct incomplete;

template <typename T> void f(T) { }

namespace naive {
    template <typename T>
    struct wrap { using type = T; };
}

namespace working {
    template <typename T>
    constexpr auto make_anon() {
        struct { using type = T; } s{};
        return s;
    }

    template <typename T>
    using wrap = decltype(make_anon<T>());
}

using working::wrap;


int main() {
    // ADL kicks in and invalid<> is instantiated
    f(wrap<invalid<>>{});

    // ADL kicks in but incomplete can't be instantiated
    f(wrap<incomplete>{});

    // ADL instantiates the types recursively
    f(wrap<wrap<invalid<>>>{});

    // ADL is disabled by parentheses
    (f)(wrap<invalid<>>{});
}
