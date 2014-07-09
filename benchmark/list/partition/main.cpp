#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>


BOOST_HANA_CONSTEXPR_LAMBDA auto is_odd = [](auto x) {
    return x % boost::hana::int_<2> == boost::hana::int_<0>;
};

int main() {
    auto xs = list(<%= (0..x).map{ |i| "boost::hana::int_<#{i}>" }.join(', ') %>);
    auto parts = partition(is_odd, xs);
}
