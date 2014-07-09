<%= render(instance) %>

#include <boost/hana/functor.hpp>


template <typename X>
struct result { };

struct f {
    template <typename X>
    constexpr result<X> operator()(X) const { return {}; }
};

template <int> struct x { };

int main() {
    auto xs = list(<%= (0..x).map { |i| "x<#{i}>{}" }.join(', ') %>);
    auto go = boost::hana::fmap(f{}, xs);
}