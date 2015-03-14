/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/comonad.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/lazy.hpp>

#include <sstream>
using namespace boost::hana;


int main() {

{

//! [extract]
static_assert(extract(lazy(1)) == 1, "");
static_assert(extract(lazy(succ)(3)) == 4, "");
//! [extract]

}{

//! [extend]
std::stringstream s; s << "1 2 3";
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
//! [extend]

}{

//! [duplicate]
static_assert(extract(extract(duplicate(lazy(3)))) == 3, "");
//! [duplicate]

}

}
