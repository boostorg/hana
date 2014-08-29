/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/lazy_comparable.hpp>

#include <test/laws/functor.hpp>
using namespace boost::hana;


int main() {
    using test::x;
    auto f = test::injection([]{});

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(lazy(x<0>), f),
            lazy(f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(lazy(x<1>), f),
            lazy(f(x<1>))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Functor_laws(
            lazy(x<0>), lazy(x<1>), lazy(x<2>)
        ));
    }
}
