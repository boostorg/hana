/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>

#include <cassert>
#include <type_traits>
using namespace boost::hana;


namespace purely_runtime_predicate {
    struct odd {
        template <typename T>
        bool operator()(T t) const { return t % 2; }
    };

    void test() {
        assert(any(odd{}, list(1, 2)));
        assert(!any(odd{}, list(2, 4)));
    }
}

namespace constexpr_predicate {
    struct odd {
        template <typename T>
        constexpr bool operator()(T t) const { return t % 2; }
    };

    static_assert(any(odd{}, list(1, 2)), "");
    static_assert(!any(odd{}, list(2, 4)), "");
}

namespace type_only_predicate {
    struct odd {
        template <typename T>
        constexpr auto operator()(T t) const { return t % int_<2>; }
    };

    static_assert(std::is_same<
        decltype(any(odd{}, list(int_<1>, int_<2>))),
        Bool<true>
    >::value, "");

    static_assert(std::is_same<
        decltype(any(odd{}, list(int_<2>, int_<4>))),
        Bool<false>
    >::value, "");
}

int main() {
    purely_runtime_predicate::test();
}
