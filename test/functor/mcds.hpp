/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional/always.hpp>

#include <test/laws/functor.hpp>
#include <test/cnumeric/logical.hpp>
#include <test/identity/comparable.hpp>
#include <test/identity/functor.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto functor = test::identity;
    using F = test::Identity;

    using test::x;
    using test::cnumeric;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // adjust
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            adjust(functor(x<0>), always(cnumeric<bool, true>), f),
            functor(f(x<0>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            adjust(functor(x<0>), always(cnumeric<bool, false>), f),
            functor(x<0>)
        ));
    }

    // fill
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            fill(functor(x<0>), x<1>),
            functor(x<1>)
        ));
    }

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(functor(x<0>), f),
            functor(f(x<0>))
        ));
    }

    // replace
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            replace(functor(x<0>), always(cnumeric<bool, true>), x<1>),
            functor(x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            replace(functor(x<0>), always(cnumeric<bool, false>), x<1>),
            functor(x<0>)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Functor_laws(
            functor(x<0>),
            functor(x<1>),
            functor(x<2>)
        ));
    }
}
