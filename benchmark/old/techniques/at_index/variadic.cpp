#include <boost/hana/detail/variadic/at.hpp>


template <int> struct x { };

int main() {
    boost::hana::detail::variadic::at<<%= input_size %>>(
        <%= (0..input_size).map { |n| "x<#{n}>{}" }.join(', ') %>
    );
}
