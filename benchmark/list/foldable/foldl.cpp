<%= render(instance) %>

#include <boost/hana/foldable.hpp>


template <typename ...>
struct result { };

struct f {
    template <typename State, typename X>
    constexpr result<State, X> operator()(State, X) const { return {}; }
};

struct state { };
template <int> struct x { };

int main() {
    auto xs = list(<%= (0..x).map { |i| "x<#{i}>{}" }.join(', ') %>);
    auto go = boost::hana::foldl(f{}, state{}, xs);
}
