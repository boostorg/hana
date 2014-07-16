<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/foldable/foldable.hpp>


template <int> struct x { };

int main() {
    boost::hana::unpack([](auto ...xs) { },
        <%= list(
            (0..x).map { |i| "x<#{i}>" },
            (0..x).map { |i| "x<#{i}>{}" }
        ) %>
    );
}
