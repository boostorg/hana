/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto i) { return i + int_<1>; };

int main() {
    BOOST_HANA_STATIC_ASSERT(fmap(f, range(int_<0>, int_<0>)) == list());
    BOOST_HANA_STATIC_ASSERT(fmap(f, range(int_<0>, int_<1>)) == list(f(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, range(int_<0>, int_<2>)) == list(f(int_<0>), f(int_<1>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, range(int_<0>, int_<3>)) == list(f(int_<0>), f(int_<1>), f(int_<2>)));
}
