#include <boost/hana/list.hpp>
using namespace boost::hana;


template <int i> struct x { };

int main() {
    permutations(list(
        <%= (0..x).map { |i| "x<#{i}>{}" }.join(', ') %>
    ));
}
