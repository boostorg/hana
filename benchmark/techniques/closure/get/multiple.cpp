/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>


using boost::hana::detail::closure;
using boost::hana::detail::get;

//////////////////////////////////////////////////////////////////////////////

template <int> struct x { };

int main() {
    closure<
        <%= (0..input_size).map { |n| "x<#{n}>" }.join(', ') %>
    > tuple;

    <% (0..(input_size / 8)).each do |n| %>
        get<
            <%= 4*n %>
        >(tuple);
    <% end %>
}
