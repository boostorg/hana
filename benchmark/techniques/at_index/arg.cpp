#include <boost/hana/functional/arg.hpp>


template <int> struct x { };

int main() {
    boost::hana::arg<<%= input_size %>>(
        <%= (0..input_size).map { |n| "x<#{n}>{}" }.join(', ') %>
    );
}
