#include <boost/hana/integral.hpp>


int main() {
    auto xs = <%= list(
        (0..x).map{ |i| "decltype(boost::hana::int_<#{i}>)" },
        (0..x).map{ |i| "boost::hana::int_<#{i}>" }
    ) %>;

    auto parts = partition(xs, [](auto x) {
        return x % boost::hana::int_<2> == boost::hana::int_<0>;
    });
}
