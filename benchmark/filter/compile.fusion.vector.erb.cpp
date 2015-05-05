/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

<% if input_size > 10 %>
    #define FUSION_MAX_VECTOR_SIZE <%= ((input_size + 9) / 10) * 10 %>
<% end %>

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/filter_if.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/mpl/integral_c.hpp>
namespace fusion = boost::fusion;


struct is_even {
    template <typename N>
    using apply = boost::mpl::integral_c<bool, N::value % 2 == 0>;
};

int main() {
    auto vector = fusion::make_vector(
        <%= (1..input_size).map { |n|
            "boost::mpl::integral_c<int, #{n}>{}"
        }.join(', ') %>
    );
    auto result = fusion::as_vector(fusion::filter_if<is_even>(vector));
    (void)result;
}
