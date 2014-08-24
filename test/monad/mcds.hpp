/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/monad/operators.hpp>

#include <test/laws/monad.hpp>
#include <test/identity/comparable.hpp>
#include <test/identity/monad.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <> struct enabled_operators<test::Identity> : Monad { };
}}

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto monad = test::identity;
    using M = test::Identity;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = compose(monad, test::injection([]{}));
    using test::x;

    // bind
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            bind(monad(x<1>), f),
            f(x<1>)
        ));
    }

    // tap
    {
        bool executed = false;
        BOOST_HANA_CONSTEXPR_LAMBDA auto exec = [&](auto) { executed = true; };
        BOOST_HANA_CONSTANT_ASSERT(equal(
            bind(monad(x<0>), tap<M>(exec)),
            monad(x<0>)
        ));
        BOOST_HANA_RUNTIME_ASSERT(executed);
    }

    // then
    {
        struct invalid { };
        BOOST_HANA_CONSTANT_ASSERT(equal(
            then(monad(invalid{}), monad(x<0>)),
            monad(x<0>)
        ));
    }

    // operators
    {
        using namespace boost::hana::operators;
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = compose(monad, test::injection([]{}));
        BOOST_HANA_CONSTEXPR_LAMBDA auto g = compose(monad, test::injection([]{}));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            monad(x<1>) | f,
            bind(monad(x<1>), f)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            monad(x<1>) | f | g,
            bind(bind(monad(x<1>), f), g)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Monad_laws<M>
            // monads
            (monad(x<0>), monad(x<1>), monad(x<2>))

            // objects
            (x<0>, x<1>, x<2>, x<3>)
        );
    }
}
