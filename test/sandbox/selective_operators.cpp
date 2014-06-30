/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <cassert>
#include <string>


namespace Comparable_operators {
    template <typename T, typename U>
    std::string operator==(T t, U u) { return "=="; }

    template <typename T, typename U>
    std::string operator!=(T t, U u) { return "!="; }

    struct enable { };
}
struct Comparable {
    using operators = Comparable_operators::enable;
};


namespace Monad_operators {
    template <typename M, typename F>
    std::string operator>>=(M m, F f) { return ">>="; }

    struct enable { };
}
struct Monad {
    using operators = Monad_operators::enable;
};



template <typename ...T>
struct enable_adl : T... { };

template <typename ...Typeclasses>
using operators = enable_adl<typename Typeclasses::operators...>;


struct Foo : operators<Comparable, Monad> { };

int main() {
    Foo a, b;
    assert((a == b) == "==");
    assert((a != b) == "!=");
    assert((a >>= b) == ">>=");
}
