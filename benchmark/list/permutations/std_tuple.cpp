#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/list.hpp>

#include <tuple>
using namespace boost::hana;


template <int i> struct x { };

int main() {
    permutations(std::make_tuple(
        <%= (0..x).map { |i| "x<#{i}>{}" }.join(', ') %>
    ));
}
