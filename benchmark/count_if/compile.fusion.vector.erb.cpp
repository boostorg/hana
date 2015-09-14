/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

<% if input_size > 10 %>
    #define FUSION_MAX_VECTOR_SIZE <%= ((input_size + 9) / 10) * 10 %>
<% end %>

#include <boost/fusion/include/count_if.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/mod.hpp>


struct is_even {
    template <typename N>
    constexpr auto operator()(N n) const {
        return n % boost::hana::int_c<2> == boost::hana::int_c<0>;
    }
};

int main() {
    auto vector = boost::fusion::make_vector(
        <%= (1..input_size).map { |n| "boost::hana::int_c<#{n}>" }.join(', ') %>
    );
    auto result = boost::fusion::count_if(vector, is_even{});
    (void)result;
}
