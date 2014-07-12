<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>


int main() {
    auto go = boost::hana::elem(boost::hana::int_<  <%= x / 2 %>    >,
        <%= list(
            (0..x).map { |i| "decltype(boost::hana::int_<#{i}>)" },
            (0..x).map { |i| "boost::hana::int_<#{i}>" }
        ) %>
    );
}
