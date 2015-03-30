/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#define FUSION_MAX_VECTOR_SIZE 20
#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/make_vector.hpp>


struct f {
    template <typename State, typename X>
    constexpr X operator()(State, X x) const { return x; }
};

struct state { };

struct x { };

int main() {
    auto vector = boost::fusion::make_vector(
        <%= (1..input_size).map { "x{}" }.join(', ') %>
    );
    auto result = boost::fusion::fold(vector, state{}, f{});
    (void)result;
}
