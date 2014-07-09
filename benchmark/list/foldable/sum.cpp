<% render(instance) %>
<%= includes(0..x) %>

#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>


<% xs = (0..x).map { |i| "boost::hana::int_<#{i}>" } %>

int main() {
    auto go = boost::hana::sum(
        <%= list(xs.map { |x| "decltype(#{x})" }, xs) %>
    );
}
