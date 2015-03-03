/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <utility>


template <int i> struct x { };

template <typename T>
void f(T&& t) {
    <% if use_forward %>
        (void)std::forward<T>(t);
    <% end %>
}

int main() {
    <% input_size.times do |n| %>
        f(x< <%= n %> >{});
    <% end %>
}
