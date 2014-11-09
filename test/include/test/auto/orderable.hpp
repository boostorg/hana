/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_ORDERABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_ORDERABLE_HPP

#include <boost/hana/orderable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/logical.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename Ord>
    auto laws<Orderable, Ord> = [] {
        auto incomparable = [](auto x, auto y) {
            return not_(or_(less(x, y), less(y, x)));
        };

        for_each(objects<Ord>, [=](auto a) {
            BOOST_HANA_CHECK(not_(less(a, a)));

            for_each(objects<Ord>, [=](auto b) {
                BOOST_HANA_CHECK(
                    less(a, b) ^implies^ not_(less(b, a))
                );

                for_each(objects<Ord>, [=](auto c) {
                    BOOST_HANA_CHECK(
                        and_(less(a, b), less(b, c)) ^implies^ less(a, c)
                    );

                    BOOST_HANA_CHECK(
                        and_(incomparable(a, b), incomparable(b, c))
                        ^implies^ incomparable(a, c)
                    );
                });
            });
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_ORDERABLE_HPP
