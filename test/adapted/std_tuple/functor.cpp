/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_tuple.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto i) { return i + int_<1>; };

int main() {
    BOOST_HANA_STATIC_ASSERT(fmap(f, std::make_tuple()) == std::make_tuple());
    BOOST_HANA_STATIC_ASSERT(fmap(f, std::make_tuple(int_<1>)) == std::make_tuple(f(int_<1>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, std::make_tuple(int_<1>, int_<2>)) == std::make_tuple(f(int_<1>), f(int_<2>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, std::make_tuple(int_<1>, int_<2>, int_<3>)) == std::make_tuple(f(int_<1>), f(int_<2>), f(int_<3>)));
}
