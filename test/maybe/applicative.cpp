/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    auto f = test::injection([]{});
    auto x = test::injection([]{})();

    // ap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(ap(nothing, nothing), nothing));
        BOOST_HANA_CONSTANT_ASSERT(equal(ap(just(f), nothing), nothing));
        BOOST_HANA_CONSTANT_ASSERT(equal(ap(nothing, just(x)), nothing));
        BOOST_HANA_CONSTANT_ASSERT(equal(ap(just(f), just(x)), just(f(x))));
    }

    // lift
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(lift<Maybe>(x), just(x)));
    }
}
