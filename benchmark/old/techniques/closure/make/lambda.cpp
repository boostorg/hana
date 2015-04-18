/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

auto closure = [](auto ...xs) {
    return [=](auto f) { return f(xs...); };
};

//////////////////////////////////////////////////////////////////////////////

template <int> struct x { };

int main() {
    <% (0..input_size).step(2).each do |n| %>
        {
            (void)closure(
                <%= (0..n).to_a.reverse.map { |i| "x<#{i}>{}" }.join(', ') %>
            );
        }
    <% end %>
}
