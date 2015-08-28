/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <utility>
namespace hana = boost::hana;


// Make sure that the tuple(Yn&&...) is not preferred over the
// tuple(tuple<Yn...> const&) and the tuple(tuple<Yn...>&&)
// constructors when copy-constructing a tuple with a single
// element that can be constructed from tuple<Yn...> const& and
// tuple<Yn...>&&, respectively.

struct trap_construct {
    trap_construct() = default;
    trap_construct(trap_construct const&) = default;
    trap_construct(trap_construct&) = default;
    trap_construct(trap_construct&&) = default;

    template <typename X>
    trap_construct(X&&) {
        static_assert(sizeof(X) && false,
        "this constructor must not be instantiated");
    }
};

int main() {
    hana::tuple<trap_construct> tuple{};
    hana::tuple<trap_construct> implicit_copy = tuple;
    hana::tuple<trap_construct> explicit_copy(tuple);
    hana::tuple<trap_construct> implicit_move = std::move(tuple);
    hana::tuple<trap_construct> explicit_move(std::move(tuple));

    (void)implicit_copy;
    (void)explicit_copy;
    (void)implicit_move;
    (void)explicit_move;
}
