/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/at.hpp>

#include <cstddef>


template <std::size_t n, typename Closure>
constexpr decltype(auto) get(Closure const& closure) {
    return closure(boost::hana::detail::variadic::at<n>);
}

auto closure = [](auto ...xs) {
    return [=](auto f) { return f(xs...); };
};

//////////////////////////////////////////////////////////////////////////////

template <int> struct x { };

int main() {
    auto tuple = closure(
        <%= (0..input_size).map { |n| "x<#{n}>{}" }.join(', ') %>
    );

    <% (0..(input_size / 8)).each do |n| %>
        get<
            <%= 4*n %>
        >(tuple);
    <% end %>
}
