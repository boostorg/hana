#include <boost/hana/list.hpp>


template <typename X>
struct result { };

constexpr struct {
    template <typename X>
    constexpr result<X> operator()(X) const;
} f{};

template <int>
struct x { };

int main() {
    auto go = boost::hana::fmap(f,
        boost::hana::list(
            <%= 0.upto(x).map { |i| "x<#{i}>{}" }.join(', ') %>
        )
    );
}
