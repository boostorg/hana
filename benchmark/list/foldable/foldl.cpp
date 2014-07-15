<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/foldable/foldable.hpp>


template <typename ...>
struct result { };

struct f {
    template <typename State, typename X>
    constexpr result<State, X> operator()(State, X) const { return {}; }
};

struct state { };
template <int> struct x { };

int main() {
    auto go = boost::hana::foldl(f{}, state{},
        <%= list(
            (0..x).map { |i| "x<#{i}>" },
            (0..x).map { |i| "x<#{i}>{}" }
        ) %>
    );
}
