/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>


using boost::hana::detail::closure_t;

//////////////////////////////////////////////////////////////////////////////

template <int> struct x { };

int main() {
    <% (0..input_size).step(2).each do |n| %>
        {
            closure_t<
                <%= (0..n).to_a.reverse.map { |i| "x<#{i}>" }.join(', ') %>
            > c;
        }
    <% end %>
}
