/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either/comparable.hpp>
#include <boost/hana/either/monad.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/laws/monad.hpp>
using namespace boost::hana;


int main() {
    auto x = test::x<0>;
    auto y = test::x<1>;

    // flatten
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            flatten(left(left(x))),
            left(left(x))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            flatten(left(right(x))),
            left(right(x))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            flatten(right(left(x))),
            left(x)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            flatten(right(right(x))),
            right(x)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Monad_laws<Either>
            // monads
            (
                left(x), left(y),
                right(x), right(y)
            )

            // objects
            (x, y)
        );
    }
}
