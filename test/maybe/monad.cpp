/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe/comparable.hpp>
#include <boost/hana/maybe/monad.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/laws/monad.hpp>
using namespace boost::hana;


int main() {
    using test::x;

    // flatten
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(flatten(nothing), nothing));
        BOOST_HANA_CONSTANT_ASSERT(equal(flatten(just(nothing)), nothing));
        BOOST_HANA_CONSTANT_ASSERT(equal(flatten(just(just(x<0>))), just(x<0>)));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Monad_laws<Maybe>
            // monads
            (
                nothing,
                just(x<0>),
                just(x<1>)
            )

            // objects
            (
                x<0>, x<1>, x<2>
            )
        );
    }
}
