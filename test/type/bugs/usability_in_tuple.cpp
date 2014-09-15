/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


// The fact that a reference to `type<...>` is returned from `head(types)` &
// friends makes it impossible to use the `decltype(head(types))::type`
// pattern, because we would be trying to fetch `::type` inside a reference.
//
// While this is not a bug strictly speaking, it is a major annoyance that
// ought to be fixed, hence this test case.

struct T; struct U; struct V;

int main() {
    auto types = tuple(type<T>, type<U>, type<V>);

    static_assert(std::is_same<
        decltype(head(types))::type, T
    >{}, "");

    static_assert(std::is_same<
        decltype(at(int_<1>, types))::type, U
    >{}, "");

    static_assert(std::is_same<
        decltype(last(types))::type, V
    >{}, "");
}
