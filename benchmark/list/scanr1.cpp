<% render(instance) %>
<%= includes((1..x+1).size) %>

#include <boost/hana/list/list.hpp>


template <typename ...>
struct result { };

struct f {
    template <typename X, typename State>
    constexpr result<X, State> operator()(X, State) const { return {}; }
};

template <int> struct x { };

int main() {
    auto go = boost::hana::scanr1(f{},
        <%= list(
            (1..x+1).map { |i| "x<#{i}>" },
            (1..x+1).map { |i| "x<#{i}>{}" }
        ) %>
    );
}
