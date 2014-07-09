<% render(instance) %>
<%= includes(0..x) %>

#include <boost/hana/iterable.hpp>


template <int> struct x { };

int main() {
    auto go = boost::hana::last(<%= list(
        (0..x).map { |i| "x<#{i}>" },
        (0..x).map { |i| "x<#{i}>{}" }
    ) %> );
}
