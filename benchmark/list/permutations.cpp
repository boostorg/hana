<% render(instance) %>
<%= includes(0..x) %>

#include <boost/hana/list.hpp>


template <int i> struct x { };

int main() {
    auto go = boost::hana::permutations(
        <%= list(
            (0..x).map { |i| "x<#{i}>" },
            (0..x).map { |i| "x<#{i}>{}" }
        ) %>
    );
}
