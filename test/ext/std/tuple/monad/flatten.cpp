/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto tuple = [](auto ...xs) {
    return std::make_tuple(xs...);
};

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(flatten(tuple(tuple(), tuple())) == tuple());
    BOOST_HANA_CONSTEXPR_ASSERT(flatten(tuple(tuple(int_<0>), tuple())) == tuple(int_<0>));
    BOOST_HANA_CONSTEXPR_ASSERT(flatten(tuple(tuple(), tuple(int_<0>))) == tuple(int_<0>));
    BOOST_HANA_CONSTEXPR_ASSERT(flatten(tuple(tuple(int_<0>), tuple(int_<1>))) == tuple(int_<0>, int_<1>));
    BOOST_HANA_CONSTEXPR_ASSERT(
        flatten(tuple(tuple(0, int_<1>), tuple(), tuple('2', 3.3), tuple(int_<4>)))
        ==
        tuple(0, int_<1>, '2', 3.3, int_<4>)
    );
}
