#include "boost/hana.hpp"

constexpr int f() { return 12; }
constexpr int g(int) { return 42; }



// This test makes sure that overload can
// be copy constructed
int main() {
    auto a = boost::hana::overload(f,g);
    auto b(a);


    BOOST_HANA_RUNTIME_CHECK(b() == 12);
    BOOST_HANA_RUNTIME_CHECK(b(1) == 42);
}
