/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/trait.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto types = list_t<int, char, long, short, char, long, double, long>;
    BOOST_HANA_STATIC_ASSERT(count(trait::is_floating_point, types) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(count(partial(trait::is_same, type<char>), types) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(count(partial(trait::is_same, type<void>), types) == int_<0>);
}
