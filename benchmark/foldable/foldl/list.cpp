#include <boost/hana/list.hpp>


template <typename ...>
struct result { };

constexpr struct {
    template <typename State, typename X>
    constexpr result<State, X> operator()(State, X) const { return {}; }
} f{};

constexpr struct { } state{};

template <int> struct x { };

int main() {
    auto go = boost::hana::foldl(f, state, boost::hana::list(
        <%= (0..x).map { |i| "x<#{i}>{}" }.join(', ') %>
    ));
}
