/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <cstddef>


template <std::size_t n /* >= 6 */>
struct get_impl {
    template <typename Closure>
    static constexpr decltype(auto) apply(Closure const& closure) {
        return get_impl<n - Closure::size>::apply(
            static_cast<typename Closure::rest const&>(closure)
        );
    }
};

template <std::size_t n, typename Closure>
constexpr decltype(auto) get(Closure const& closure) {
    return get_impl<n>::apply(closure);
}

#define CLOSURE_GET(n)                                                      \
    template <>                                                             \
    struct get_impl<n> {                                                    \
        template <typename Closure>                                         \
        static constexpr decltype(auto) apply(Closure const& closure) {     \
            return closure.x##n;                                            \
        }                                                                   \
    }                                                                       \
/**/

CLOSURE_GET(0);
CLOSURE_GET(1);
CLOSURE_GET(2);
CLOSURE_GET(3);
CLOSURE_GET(4);
CLOSURE_GET(5);


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
struct closure<X0, X1, X2, X3, X4, X5, Xn...>
    : closure<Xn...>
{
    X0 x0; X1 x1; X2 x2; X3 x3; X4 x4; X5 x5;
    static constexpr std::size_t size = 6;
    using rest = closure<Xn...>;

    closure() = default;

    template <typename Y0, typename Y1, typename Y2, typename Y3, typename Y4, typename Y5, typename ...Yn>
    constexpr closure(Y0 y0, Y1 y1, Y2 y2, Y3 y3, Y4 y4, Y5 y5, Yn ...yn)
        : x0(y0), x1(y1), x2(y2), x3(y3), x4(y4), x5(y5),
          rest{yn...}
    { }
};

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
