<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/integral.hpp>


<% xs = (1..x+1).map { |i| "boost::hana::int_<#{i}>" } %>

int main() {
    auto go = boost::hana::product(
        <%= list(xs.map { |x| "decltype(#{x})" }, xs) %>
    );
}
