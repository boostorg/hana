#include <boost/hana/ext/mpl/vector.hpp>
#include <boost/hana/type.hpp>

<% vector = Benchcc::MPL::Vector.new((0...x).map { |i| "x<#{i}>" }) %>
<%= vector.includes %>


struct f {
    template <typename>
    struct apply { struct type; };
};

template <int> struct x;

int main() {
    auto go = boost::hana::fmap(
        boost::hana::metafunction_class<f>,
        <%= vector %>{}
    );
}
