<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/iterable/iterable.hpp>


template <int> struct x { };

int main() {
    boost::hana::for_each(
        <%= list(
            (0..x).map { |i| "x<#{i}>" },
            (0..x).map { |i| "x<#{i}>{}" }
        ) %>,
        [](auto x) { }
    );
}
