/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

<% if input_size > 10 %>
    #define FUSION_MAX_VECTOR_SIZE <%= ((input_size + 9) / 10) * 10 %>
<% end %>

#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/mpl/integral_c.hpp>


struct is_last {
    template <typename N>
    struct apply
        : boost::mpl::integral_c<bool, N::type::value == <%= input_size %>>
    { };
};

int main() {
    auto vector = boost::fusion::make_vector(
        <%= (1..input_size).map { |n|
            "boost::mpl::integral_c<int, #{n}>{}"
        }.join(', ') %>
    );
    auto result = boost::fusion::find_if<is_last>(vector);
    (void)result;
}
