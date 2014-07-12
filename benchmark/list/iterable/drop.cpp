<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>


template <int> struct x { };

int main() {
    auto go = boost::hana::drop(
        boost::hana::int_<  <%= x / 2 %>    >,
        <%= list(
            (0..x).map { |i| "x<#{i}>" },
            (0..x).map { |i| "x<#{i}>{}" }
        ) %>
    );
}
