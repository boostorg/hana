/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/push_back.hpp>


struct f {
    template <typename State, typename X>
    constexpr X operator()(State, X x) const { return x; }
};

struct state { };

template <int i>
struct x { };

int main() {
    auto vector = <%= fusion_vector((1..input_size).map { |n| "x<#{n}>{}" }) %>;
    auto result = boost::fusion::fold(vector, state{}, f{});
    (void)result;
}
