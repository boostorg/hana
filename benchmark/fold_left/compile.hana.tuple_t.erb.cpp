/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/fold_left.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>


template <typename State, typename X>
struct f { using type = X; };

struct state { };

template <int i>
struct t { };

int main() {
    constexpr auto tuple = boost::hana::tuple_t<
        <%= (1..input_size).to_a.map { |n| "t<#{n}>" }.join(', ') %>
    >;
    constexpr auto result = boost::hana::fold_left(
        tuple,
        boost::hana::type<state>,
        boost::hana::metafunction<f>
    );
    (void)result;
}
