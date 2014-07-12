<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>


// We must make sure we always have the same seed to have reproducible results.
<% xs = (0..x).map { |i| "boost::hana::int_<#{i}>" }.shuffle(random: Random.new(0)) %>

int main() {
    auto go = boost::hana::minimum(
        <%= list(xs.map { |x| "decltype(#{x})" }, xs) %>
    );
}
