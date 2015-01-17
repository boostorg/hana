/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_RING_HPP
#define BOOST_HANA_TEST_TEST_AUTO_RING_HPP

#include <boost/hana/ring.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/monoid.hpp>

// required instances
#include <test/auto/group.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename R>
    auto laws<Ring, R> = [] {
        BOOST_HANA_CONSTANT_CHECK(models<Ring, R>);
        laws<Group, R>();

        for_each(objects<R>, [](auto x) {
            BOOST_HANA_CHECK(equal(
                mult(one<R>(), x), x
            ));

            BOOST_HANA_CHECK(equal(
                mult(x, one<R>()), x
            ));

            for_each(objects<R>, [=](auto y) {
                for_each(objects<R>, [=](auto z) {
                    BOOST_HANA_CHECK(equal(
                        mult(x, mult(y, z)),
                        mult(mult(x, y), z)
                    ));

                    BOOST_HANA_CHECK(equal(
                        mult(x, plus(y, z)),
                        plus(mult(x, y), mult(x, z))
                    ));
                });
            });
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_RING_HPP
