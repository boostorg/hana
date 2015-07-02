/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/concept/enumerable.hpp>
#include <boost/hana/functional/placeholder.hpp>
#include <boost/hana/lazy.hpp>

#include <sstream>
using namespace boost::hana;


int main() {

{

//! [lazy]
BOOST_HANA_CONSTEXPR_LAMBDA auto f = lazy([](auto x) {
    return 1 / x;
});
BOOST_HANA_CONSTEXPR_LAMBDA auto g = lazy([](auto x) {
    return x + 1;
});

BOOST_HANA_CONSTEXPR_CHECK(eval(if_(false_, f(0), g(0))) == 0 + 1);
//! [lazy]

}{

//! [eval]
static_assert(eval(lazy(_ + 1)(3)) == 4, "");
//! [eval]

}{

//! [functor]
static_assert(eval(transform(lazy(4 / _)(1), _ * 3)) == (4 / 1) * 3, "");

transform(lazy(4 / _)(0), _ * 3); // never evaluated
//! [functor]

}{

//! [comonad]
std::stringstream s("1 2 3");
auto i = lazy([&] {
    int i;
    s >> i;
    return i;
})();

auto i_plus_one = extend(i, [](auto lazy_int) {
    return succ(eval(lazy_int));
});

BOOST_HANA_RUNTIME_CHECK(extract(i_plus_one) == 2);
BOOST_HANA_RUNTIME_CHECK(extract(i_plus_one) == 3);
BOOST_HANA_RUNTIME_CHECK(extract(i_plus_one) == 4);
//! [comonad]

}

}
