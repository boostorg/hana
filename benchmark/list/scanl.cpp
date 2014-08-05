<% render(instance) %>
<%= includes((0..x).size) %>

#include <boost/hana/list/list.hpp>


template <typename ...>
struct result { };

struct f {
    template <typename State, typename X>
    constexpr result<State, X> operator()(State, X) const { return {}; }
};

struct state { };
template <int> struct x { };

int main() {
    auto go = boost::hana::scanl(
        <%= list(
            (0..x).map { |i| "x<#{i}>" },
            (0..x).map { |i| "x<#{i}>{}" }
        ) %>,
        state{}, f{}
    );
}
