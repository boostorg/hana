<% render(instance) %>
<%= includes((0..x).size) %>

#include <boost/hana/foldable/foldable.hpp>


template <int> struct x { };

int main() {
    auto go = boost::hana::length(
        <%= list(
            (0..x).map { |i| "x<#{i}>" },
            (0..x).map { |i| "x<#{i}>{}" }
        ) %>
    );
}
