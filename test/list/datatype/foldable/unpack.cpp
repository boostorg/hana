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


BOOST_HANA_CONSTEXPR_LAMBDA auto tuple = [](auto ...x) {
    return std::make_tuple(x...);
};

int main() {
    BOOST_HANA_STATIC_ASSERT(unpack(tuple, list()) == tuple());
    BOOST_HANA_STATIC_ASSERT(unpack(tuple, list(int_<0>)) == tuple(int_<0>));
    BOOST_HANA_STATIC_ASSERT(unpack(tuple, list(int_<0>, int_<1>)) == tuple(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(unpack(tuple, list(int_<0>, int_<1>, int_<2>)) == tuple(int_<0>, int_<1>, int_<2>));
}
