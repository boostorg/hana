/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/wrong.hpp>
#include <boost/hana/detail/closure.hpp>

#include <utility>
using namespace boost::hana;


struct Trap {
    Trap() = default;
    Trap(Trap const&) = default;
    Trap(Trap&) = default;
    Trap(Trap&&) = default;

    template <typename X>
    Trap(X&&) {
        static_assert(detail::wrong<X>{},
        "this constructor must not be instantiated");
    }
};

int main() {
    detail::closure<Trap> t{};
    detail::closure<Trap> implicit_copy = t;
    detail::closure<Trap> explicit_copy(t);
    detail::closure<Trap> implicit_move = std::move(t);
    detail::closure<Trap> explicit_move(std::move(t));

    (void)implicit_copy;
    (void)explicit_copy;
    (void)implicit_move;
    (void)explicit_move;
}
