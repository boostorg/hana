/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <cstddef>
#include <utility>


template <std::size_t Index, typename X>
struct element { X x; };

template <typename Indices, typename ...Xn>
struct closure_impl;

template <std::size_t ...Indices, typename ...Xn>
struct closure_impl<std::index_sequence<Indices...>, Xn...>
    : element<Indices, Xn>...
{
    closure_impl() = default;

    template <typename ...Yn>
    explicit constexpr closure_impl(Yn ...yn)
        : element<Indices, Xn>(yn)...
    { }

    template <typename F>
    constexpr decltype(auto) unpack(F&& f) {
        return f(static_cast<element<Indices, Xn>&>(*this).x...);
    }
};

template <typename ...Xn>
struct closure : closure_impl<std::index_sequence_for<Xn...>, Xn...> { };

template <std::size_t n, typename Xn>
constexpr decltype(auto) get(element<n, Xn> const& elt)
{ return elt.x; }


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
