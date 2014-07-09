<% render(instance) %>
<%= includes(0..x) %>

#include <boost/hana/foldable.hpp>


template <typename ...>
struct result { };

struct f {
    template <typename State, typename X>
    constexpr result<State, X> operator()(State, X) const { return {}; }
};

template <int> struct x { };

int main() {
    auto go = boost::hana::foldl1(f{},
        <%= list(
                (1..x+1).map { |i| "x<#{i}>" },
                (1..x+1).map { |i| "x<#{i}>{}" }
        ) %>
    );
}
