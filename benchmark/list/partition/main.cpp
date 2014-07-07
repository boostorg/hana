#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
namespace hana = boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto is_odd = [](auto x) {
    return x % hana::int_<2> == hana::int_<0>;
};

int main() {
    auto xs = hana::list(<%= (0..x).map{ |i| "hana::int_<#{i}>" }.join(', ') %>);
    auto parts = partition(is_odd, xs);
}
