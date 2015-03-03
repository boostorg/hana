/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>
using namespace boost::hana;


template <int> struct x { };

int main() {
    detail::closure<
        <%= (0..input_size).map { |n| "x<#{n}>" }.join(', ') %>
    > tuple;

    <% (0..(input_size / 8)).each do |n| %>
        detail::get<
            <%= 4*n %>
        >(tuple);
    <% end %>
}
