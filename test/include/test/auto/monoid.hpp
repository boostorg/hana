/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_MONOID_HPP
#define BOOST_HANA_TEST_TEST_AUTO_MONOID_HPP

#include <boost/hana/monoid.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename M>
    auto laws<Monoid, M> = [] {
        for_each(objects<M>, [](auto x) {
            BOOST_HANA_ASSERT(equal(
                x,
                plus(zero<M>, x)
            ));

            BOOST_HANA_ASSERT(equal(
                x,
                plus(x, zero<M>)
            ));

            for_each(objects<M>, [=](auto y) {
                for_each(objects<M>, [=](auto z) {
                    BOOST_HANA_ASSERT(equal(
                        plus(x, plus(y, z)),
                        plus(plus(x, y), z)
                    ));
                });
            });
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_MONOID_HPP
