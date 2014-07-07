<% cons = Benchcc::Fusion::Cons.new((0..x).map { |i| "x<#{i}>" }) %>
<%= cons.includes %>

#include <boost/hana/ext/fusion.hpp>


template <typename ...>
struct result { };

constexpr struct {
    template <typename State, typename X>
    constexpr result<State, X> operator()(State, X) const { return {}; }
} f{};

constexpr struct { } state{};

template <int> struct x { };

int main() {
    auto go = boost::hana::foldl(f, state, <%= cons %>{});
}
