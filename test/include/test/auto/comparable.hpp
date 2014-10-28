/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_COMPARABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_COMPARABLE_HPP

#include <boost/hana/comparable.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/logical.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename T>
    auto laws<Comparable, T> = [] {
        for_each(objects<T>, [](auto a) {
            BOOST_HANA_ASSERT(equal(a, a));

            for_each(objects<T>, [=](auto b) {
                BOOST_HANA_ASSERT(
                    not_equal(a, b) ^iff^ not_(equal(a, b))
                );

                BOOST_HANA_ASSERT(
                    equal(a, b) ^implies^ equal(b, a)
                );

                for_each(objects<T>, [=](auto c) {
                    BOOST_HANA_ASSERT(
                        and_(equal(a, b), equal(b, c)) ^implies^ equal(a, c)
                    );
                });
            });
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_COMPARABLE_HPP
