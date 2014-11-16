/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/common.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename T>
struct ImplicitConvertibleTo {
    constexpr operator T() const { return {}; }
};

struct T { };

int main() {
    static_assert(std::is_same<
        common_t<T, T>, T
    >{}, "");

    static_assert(std::is_same<
        common_t<ImplicitConvertibleTo<T>, T>, T
    >{}, "");

    static_assert(std::is_same<
        common_t<T, ImplicitConvertibleTo<T>>, T
    >{}, "");
}
