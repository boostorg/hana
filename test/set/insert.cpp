/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set/comparable.hpp>
#include <boost/hana/set/insert.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    using test::x;

    BOOST_HANA_CONSTANT_ASSERT(equal(
        insert(set(), x<0>),
        set(x<0>)
    ));

    BOOST_HANA_CONSTANT_ASSERT(equal(
        insert(set(x<0>), x<0>),
        set(x<0>)
    ));

    BOOST_HANA_CONSTANT_ASSERT(equal(
        insert(set(x<0>), x<1>),
        set(x<0>, x<1>)
    ));

    BOOST_HANA_CONSTANT_ASSERT(equal(
        insert(set(x<0>, x<1>), x<1>),
        set(x<0>, x<1>)
    ));

    BOOST_HANA_CONSTANT_ASSERT(equal(
        insert(set(x<0>, x<1>), x<2>),
        set(x<0>, x<1>, x<2>)
    ));

    BOOST_HANA_CONSTANT_ASSERT(equal(
        insert(set(x<0>, x<1>, x<2>), x<3>),
        set(x<0>, x<1>, x<2>, x<3>)
    ));
}
