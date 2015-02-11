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
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>

#include <test/auto/applicative.hpp>
#include <test/auto/base.hpp>
#include <test/injection.hpp>


namespace boost { namespace hana { namespace test {
    template <typename M>
    auto laws<Monad, M> = [] {
        laws<Applicative, M>();

        auto f = compose(lift<M>, injection([]{}));
        auto g = compose(lift<M>, injection([]{}));
        auto h = compose(lift<M>, injection([]{}));
        auto x = injection([]{})();

        for_each(objects<M>, [=](auto m) {
            // Laws formulated with mcompose
            {
                // left identity
                BOOST_HANA_CHECK(equal(
                    mcompose<M>(lift<M>, f)(x),
                    f(x)
                ));

                // right identity
                BOOST_HANA_CHECK(equal(
                    mcompose<M>(f, lift<M>)(x),
                    f(x)
                ));

                // associativity
                BOOST_HANA_CHECK(equal(
                    mcompose<M>(mcompose<M>(f, g), h)(x),
                    mcompose<M>(f, mcompose<M>(g, h))(x)
                ));
            }

            // Laws formulated with bind
            {
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
                    transform(m, f),
                    bind(m, compose(lift<M>, f))
                ));
            }

            // Consistency of the method definitions
            //
            // NOTE: The `flatten` test below requires the objects to be
            // Monad<Monad<T>>, so it is disabled right now.
#if 0
            BOOST_HANA_CHECK(equal(
                bind(m, f),
                flatten(transform(m, f))
            ));

            BOOST_HANA_CHECK(equal(
                flatten(m),
                bind(m, id)
            ));
#endif
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_MONAD_HPP
