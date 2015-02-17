/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/auto/foldable.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    // Foldable
    {
        int a[] = {1};
        int b[] = {1, 2};
        int c[] = {1, 2, 3};
        int d[] = {1, 2, 3, 4};

        // unpack
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});


            BOOST_HANA_CONSTEXPR_CHECK(equal(
                unpack(a, f),
                f(1)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                unpack(b, f),
                f(1, 2)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                unpack(c, f),
                f(1, 2, 3)
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                unpack(d, f),
                f(1, 2, 3, 4)
            ));
        }
    }
}
