/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_MONAD_HPP
#define BOOST_HANA_TEST_TEST_AUTO_MONAD_HPP

#include <boost/hana/monad.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functor.hpp>

// required instances
#include <test/auto/applicative.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>


namespace boost { namespace hana { namespace test {
    template <typename M>
    auto laws<Monad, M> = [] {
        BOOST_HANA_CONSTANT_CHECK(models<Monad, M>);

        // Instance-wide laws
        {
            auto f = compose(lift<M>, injection([]{}));
            auto g = compose(lift<M>, injection([]{}));
            auto x = injection([]{})();

            for_each(objects<M>, [=](auto m) {
                BOOST_HANA_CHECK(equal(
                    bind(lift<M>(x), f),
                    f(x)
                ));

                BOOST_HANA_CHECK(equal(
                    bind(m, lift<M>),
                    m
                ));

                BOOST_HANA_CHECK(equal(
                    bind(m, [=](auto x) { return bind(f(x), g); }),
                    bind(bind(m, f), g)
                ));

                BOOST_HANA_CHECK(equal(
                    fmap(m, f),
                    bind(m, compose(lift<M>, f))
                ));
            });
        }

        // Applicative instance
        {
            laws<Applicative, M>();

            // ...
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_MONAD_HPP
