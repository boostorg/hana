/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


// The fact that a reference to `type<...>` is returned from `head(types)` &
// friends used to break the `decltype(head(types))::type` pattern, because
// we would be trying to fetch `::type` inside a reference. To work around
// this, a unary `operator+` turning a lvalue `Type` into a rvalue `Type`
// was added.

struct T; struct U; struct V;

int main() {
    auto check = [](auto types) {
        static_assert(std::is_same<
            typename decltype(+head(types))::type, T
        >{}, "");

        static_assert(std::is_same<
            typename decltype(+at(types, int_<1>))::type, U
        >{}, "");

        static_assert(std::is_same<
            typename decltype(+last(types))::type, V
        >{}, "");
    };

    check(make<Tuple>(type<T>, type<U>, type<V>));
    check(tuple_t<T, U, V>);
}
