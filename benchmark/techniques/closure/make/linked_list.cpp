/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

template <typename ...Xn>
struct closure;

template <>
struct closure<> { };

template <typename X0>
struct closure<X0> { X0 x0; };

template <typename X0, typename X1>
struct closure<X0, X1> { X0 x0; X1 x1; };

template <typename X0, typename X1, typename X2>
struct closure<X0, X1, X2> { X0 x0; X1 x1; X2 x2; };

template <typename X0, typename X1, typename X2, typename X3>
struct closure<X0, X1, X2, X3> { X0 x0; X1 x1; X2 x2; X3 x3; };

template <typename X0, typename X1, typename X2, typename X3, typename X4>
struct closure<X0, X1, X2, X3, X4> { X0 x0; X1 x1; X2 x2; X3 x3; X4 x4; };

template <typename X0, typename X1, typename X2, typename X3, typename X4, typename X5, typename ...Xn>
struct closure<X0, X1, X2, X3, X4, X5, Xn...> : closure<Xn...>
{ X0 x0; X1 x1; X2 x2; X3 x3; X4 x4; X5 x5; };

//////////////////////////////////////////////////////////////////////////////

template <int> struct x { };

int main() {
    <% (0..input_size).step(2).each do |n| %>
        {
            closure<
                <%= (0..n).to_a.reverse.map { |i| "x<#{i}>" }.join(', ') %>
            > c;
        }
    <% end %>
}
