#include <boost/hana/ext/std/tuple.hpp>


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
        std::make_tuple(
            <%= 0.upto(x).map { |i| "x<#{i}>{}" }.join(', ') %>
        )
    );
}
