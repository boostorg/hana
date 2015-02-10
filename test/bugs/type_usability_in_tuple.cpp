/*
@copyright Louis Dionne 2014
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
    auto types = tuple(type<T>, type<U>, type<V>);

    static_assert(std::is_same<
        decltype(+head(types))::type, T
    >{}, "");

    static_assert(std::is_same<
        decltype(+at(int_<1>, types))::type, U
    >{}, "");

    static_assert(std::is_same<
        decltype(+last(types))::type, V
    >{}, "");
}
