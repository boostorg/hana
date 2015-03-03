/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_MONAD_PLUS_HPP
#define BOOST_HANA_TEST_TEST_AUTO_MONAD_PLUS_HPP

#include <boost/hana/monad_plus.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functor.hpp>

#include <test/auto/monad.hpp>
#include <test/auto/base.hpp>
#include <test/injection.hpp>


namespace boost { namespace hana { namespace test {
    template <typename M>
    auto laws<MonadPlus, M> = [] {
        laws<Monad, M>();
        static_assert(models<MonadPlus(M)>{}, "");

        auto f = compose(lift<M>, injection([]{}));

        for_each(objects<M>, [=](auto a) {
        for_each(objects<M>, [=](auto b) {
        for_each(objects<M>, [=](auto c) {
            // left identity
            BOOST_HANA_CHECK(equal(
                concat(empty<M>(), a),
                a
            ));

            // right identity
            BOOST_HANA_CHECK(equal(
                concat(a, empty<M>()),
                a
            ));

            // associativity
            BOOST_HANA_CHECK(equal(
                concat(a, concat(b, c)),
                concat(concat(a, b), c)
            ));

            // absorption
            BOOST_HANA_CHECK(equal(
                bind(empty<M>(), f),
                empty<M>()
            ));

            BOOST_HANA_CHECK(equal(
                bind(a, always(empty<M>())),
                empty<M>()
            ));
        });});});
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_MONAD_PLUS_HPP
