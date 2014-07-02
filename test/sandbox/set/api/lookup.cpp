/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/set.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(lookup(int_<1>, set()) == nothing);

    BOOST_HANA_STATIC_ASSERT(lookup(int_<1>, set(int_<1>)) == just(int_<1>));

    BOOST_HANA_STATIC_ASSERT(lookup(int_<1>, set(int_<1>, int_<2>)) == just(int_<1>));
    BOOST_HANA_STATIC_ASSERT(lookup(int_<2>, set(int_<1>, int_<2>)) == just(int_<2>));
    BOOST_HANA_STATIC_ASSERT(lookup(int_<3>, set(int_<1>, int_<2>)) == nothing);

    BOOST_HANA_STATIC_ASSERT(lookup(int_<1>, set(int_<1>, int_<2>, int_<3>)) == just(int_<1>));
    BOOST_HANA_STATIC_ASSERT(lookup(int_<2>, set(int_<1>, int_<2>, int_<3>)) == just(int_<2>));
    BOOST_HANA_STATIC_ASSERT(lookup(int_<3>, set(int_<1>, int_<2>, int_<3>)) == just(int_<3>));
    BOOST_HANA_STATIC_ASSERT(lookup(int_<4>, set(int_<1>, int_<2>, int_<3>)) == nothing);
}
