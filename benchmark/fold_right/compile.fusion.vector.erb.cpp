/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

<% if input_size > 10 %>
    #define FUSION_MAX_VECTOR_SIZE <%= ((input_size + 9) / 10) * 10 %>
<% end %>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/reverse_fold.hpp>
namespace fusion = boost::fusion;


struct f {
    template <typename State, typename X>
    constexpr X operator()(State, X x) const { return x; }
};

struct state { };

template <int i>
struct x { };

int main() {
    auto vector = fusion::make_vector(
        <%= (1..input_size).map { |n| "x<#{n}>{}" }.join(', ') %>
    );
    auto result = boost::fusion::reverse_fold(vector, state{}, f{});
    (void)result;
}
