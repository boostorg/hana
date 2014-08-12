/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>

#include <test/laws/functor.hpp>
using namespace boost::hana;


int main() {
    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, nothing), nothing));
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, just(x<0>)), just(f(x<0>))));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Functor_laws(
            nothing, just(x<0>), just(x<1>)
        ));
    }
}
