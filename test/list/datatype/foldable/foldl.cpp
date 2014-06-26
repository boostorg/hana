/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto y) {
    return std::make_tuple(x, y);
};

int main() {
    BOOST_HANA_STATIC_ASSERT(foldl(f, int_<0>, list()) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(foldl(f, int_<0>, list(int_<1>)) == f(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(foldl(f, int_<0>, list(int_<1>, int_<2>)) == f(f(int_<0>, int_<1>), int_<2>));
    BOOST_HANA_STATIC_ASSERT(foldl(f, int_<0>, list(int_<1>, int_<2>, int_<3>)) == f(f(f(int_<0>, int_<1>), int_<2>), int_<3>));
}
