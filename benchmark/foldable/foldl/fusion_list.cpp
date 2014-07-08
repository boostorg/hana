<% list = Benchcc::Fusion::List.new((0..x).map { |i| "x<#{i}>" }) %>
<%= list.includes %>

#include <boost/hana/ext/boost/fusion.hpp>


template <typename ...>
struct result { };

constexpr struct {
    template <typename State, typename X>
    constexpr result<State, X> operator()(State, X) const { return {}; }
} f{};

constexpr struct { } state{};

template <int> struct x { };

int main() {
    auto go = boost::hana::foldl(f, state, <%= list %>{});
}
