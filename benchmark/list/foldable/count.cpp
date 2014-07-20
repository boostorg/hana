<% render(instance) %>
<%= includes((0..x).size) %>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/integral.hpp>


BOOST_HANA_CONSTEXPR_LAMBDA auto is_odd = [](auto x) {
    return x % boost::hana::int_<2> != boost::hana::int_<0>;
};

int main() {
    auto go = boost::hana::count(is_odd,
        <%= list(
            (0..x).map { |i| "decltype(boost::hana::int_<#{i}>)" },
            (0..x).map { |i| "boost::hana::int_<#{i}>" }
        ) %>
    );
}
