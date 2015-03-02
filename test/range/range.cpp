/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // range_c
    //////////////////////////////////////////////////////////////////////////
    {
        using T = int;
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 0>,
            range(integral_constant<T, 0>, integral_constant<T, 0>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 1>,
            range(integral_constant<T, 0>, integral_constant<T, 1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 0, 2>,
            range(integral_constant<T, 0>, integral_constant<T, 2>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 1>,
            range(integral_constant<T, 1>, integral_constant<T, 1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 2>,
            range(integral_constant<T, 1>, integral_constant<T, 2>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            range_c<T, 1, 3>,
            range(integral_constant<T, 1>, integral_constant<T, 3>)
        ));
    }
}
