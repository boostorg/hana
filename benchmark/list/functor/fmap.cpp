<% render(instance) %>
<%= includes((0..x).size) %>

#include <boost/hana/functor/functor.hpp>


template <typename X>
struct result { };

struct f {
    template <typename X>
    constexpr result<X> operator()(X) const { return {}; }
};

template <int> struct x { };

int main() {
    auto go = boost::hana::fmap(
        <%= list((0..x).map { |i| "x<#{i}>" }, (0..x).map { |i| "x<#{i}>{}" }) %>,
        f{}
    );
}
