<% render(instance) %>
<%= includes((0..x).size) %>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/searchable/searchable.hpp>


BOOST_HANA_CONSTEXPR_LAMBDA auto in_the_middle = [](auto x) {
    return x == boost::hana::int_<  <%= x / 2 %>    >;
};

int main() {
    auto go = boost::hana::find(in_the_middle,
        <%= list(
            (0..x).map { |i| "decltype(boost::hana::int_<#{i}>)" },
            (0..x).map { |i| "boost::hana::int_<#{i}>" }
        ) %>
    );
}
