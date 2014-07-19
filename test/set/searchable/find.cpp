/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = int_<i>;

BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

int main() {
    BOOST_HANA_STATIC_ASSERT(find(is(x<1>), set()) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(is(x<1>), set(x<1>)) == just(x<1>));
    BOOST_HANA_STATIC_ASSERT(find(is(x<2>), set(x<1>)) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(is(x<1>), set(x<1>, x<2>)) == just(x<1>));
    BOOST_HANA_STATIC_ASSERT(find(is(x<2>), set(x<1>, x<2>)) == just(x<2>));
    BOOST_HANA_STATIC_ASSERT(find(is(x<3>), set(x<1>, x<2>)) == nothing);
}
