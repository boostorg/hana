/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foreign/comparable.hpp>
#include <boost/hana/foreign/foldable.hpp>
#include <boost/hana/foreign/logical.hpp> // for laws

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/laws/foldable.hpp>
using namespace boost::hana;


int main() {
    int a[] = {1};
    int b[] = {1, 2};
    int c[] = {1, 2, 3};
    int d[] = {1, 2, 3, 4};

    // unpack
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});


        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            unpack(a, f),
            f(1)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            unpack(b, f),
            f(1, 2)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            unpack(c, f),
            f(1, 2, 3)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            unpack(d, f),
            f(1, 2, 3, 4)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Foldable_laws(
            a, b, c, d
        ));
    }
}
