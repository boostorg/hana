<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/iterable.hpp>


BOOST_HANA_CONSTEXPR_LAMBDA auto is_odd = [](auto x) {
    return x % boost::hana::int_<2> != boost::hana::int_<0>;
};

int main() {
    auto go = boost::hana::drop_while(is_odd,
        <%= list(
            (0..x/2).map { |i| "boost::hana::int_<#{2*i+1}>" } +
            (0..x/2).map { |i| "boost::hana::int_<#{2*i}>" },

            (0..x/2).map { |i| "boost::hana::int_<#{2*i+1}>{}" } +
            (0..x/2).map { |i| "boost::hana::int_<#{2*i}>{}" }
        ) %>
    );
}
