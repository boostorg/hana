#include <boost/hana/ext/boost/mpl/list.hpp>
#include <boost/hana/type.hpp>

<% list = Benchcc::MPL::List.new((0...x).map { |i| "x<#{i}>" }) %>
<%= list.includes %>


struct f {
    template <typename>
    struct apply { struct type; };
};

template <int> struct x;

int main() {
    auto go = boost::hana::fmap(
        boost::hana::metafunction_class<f>,
        <%= list %>{}
    );
}
