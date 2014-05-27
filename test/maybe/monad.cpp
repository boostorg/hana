/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(unit<_Maybe>(int_<3>) == just(int_<3>));
    BOOST_HANA_STATIC_ASSERT(unit<_Maybe>(3) == just(3));

    BOOST_HANA_STATIC_ASSERT(join(nothing) == nothing);
    BOOST_HANA_STATIC_ASSERT(join(just(nothing)) == nothing);
    BOOST_HANA_STATIC_ASSERT(join(just(just(int_<3>))) == just(int_<3>));
}
