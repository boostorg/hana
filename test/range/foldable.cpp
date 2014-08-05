/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...xs) {
    return std::make_tuple(xs...);
};

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(unpack(range(int_<0>, int_<0>), f) == f());
    BOOST_HANA_CONSTEXPR_ASSERT(unpack(range(int_<0>, int_<1>), f) == f(int_<0>));
    BOOST_HANA_CONSTEXPR_ASSERT(unpack(range(int_<0>, int_<2>), f) == f(int_<0>, int_<1>));
    BOOST_HANA_CONSTEXPR_ASSERT(unpack(range(int_<0>, int_<3>), f) == f(int_<0>, int_<1>, int_<2>));

    BOOST_HANA_CONSTANT_ASSERT(length(range(int_<0>, int_<0>)) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(length(range(int_<0>, int_<1>)) == int_<1>);
    BOOST_HANA_CONSTANT_ASSERT(length(range(int_<0>, int_<2>)) == int_<2>);
    BOOST_HANA_CONSTANT_ASSERT(length(range(int_<4>, int_<4>)) == int_<0>);
    BOOST_HANA_CONSTANT_ASSERT(length(range(int_<4>, int_<10>)) == int_<6>);
}
