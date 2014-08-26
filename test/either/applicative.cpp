/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    auto f = test::injection([]{});
    auto x = test::x<0>;
    constexpr struct { } undefined{};

    // ap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(left(x), left(undefined)),
            left(x)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(left(x), right(undefined)),
            left(x)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(right(undefined), left(x)),
            left(x)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            ap(right(f), right(x)),
            right(f(x))
        ));
    }

    // lift
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(lift<Either>(x), right(x)));
    }
}
