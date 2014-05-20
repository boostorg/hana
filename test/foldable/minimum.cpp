/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "minimal_foldable.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(minimum(foldable(int_<0>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(minimum(foldable(int_<0>, int_<1>)) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(minimum(foldable(int_<1>, int_<0>)) == int_<0>);
}
