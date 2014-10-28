/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>

#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/std/size_t.hpp>
using namespace boost::hana;


template <detail::std::size_t n, typename Xn>
constexpr decltype(auto) get(detail::closure_element<n, Xn> const& elt)
{ return elt.get; }

template <detail::std::size_t n, typename Xn>
constexpr decltype(auto) get(detail::closure_element<n, Xn>& elt)
{ return elt.get; }

template <detail::std::size_t n, typename Xn>
constexpr decltype(auto) get(detail::closure_element<n, Xn>&& elt)
{ return detail::std::move(elt.get); }


//////////////////////////////////////////////////////////////////////////////

template <int> struct x { };

int main() {
    detail::closure_t<
        <%= (0..input_size).map { |n| "x<#{n}>" }.join(', ') %>
    > tuple;

    <% (0..(input_size / 8)).each do |n| %>
        get<
            <%= 4*n %>
        >(tuple);
    <% end %>
}
