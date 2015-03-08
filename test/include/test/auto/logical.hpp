/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_LOGICAL_HPP
#define BOOST_HANA_TEST_TEST_AUTO_LOGICAL_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/logical.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename L>
    auto laws<Logical, L> = [] {
        static_assert(models<Logical, L>{}, "");

        // laws
        {
            auto is_true_valued = [](auto x) { return if_(x, true_, false_); };
            auto is_false_valued = [](auto x) { return if_(x, false_, true_); };
            auto for_each_such_that = [](auto xs, auto pred, auto f) {
                for_each(xs, [=](auto x) {
                    eval_if(pred(x),
                        [=](auto _) { _(f)(x); },
                        [](auto _) { }
                    );
                });
            };

            for_each_such_that(objects<L>, is_true_valued, [=](auto t) {
            for_each_such_that(objects<L>, is_false_valued, [=](auto f) {
            for_each(objects<L>, [=](auto a) {
            for_each(objects<L>, [=](auto b) {
            for_each(objects<L>, [=](auto c) {
                // associativity
                BOOST_HANA_CHECK(equal(
                    or_(a, or_(b, c)), or_(or_(a, b), c)
                ));
                BOOST_HANA_CHECK(equal(
                    and_(a, and_(b, c)), and_(and_(a, b), c)
                ));

                // equivalence through commutativity
                BOOST_HANA_CHECK(
                    or_(a, b) ^iff^ or_(b, a)
                );
                BOOST_HANA_CHECK(
                    and_(a, b) ^iff^ and_(b, a)
                );

                // absorption
                BOOST_HANA_CHECK(equal(
                    or_(a, and_(a, b)), a
                ));
                BOOST_HANA_CHECK(equal(
                    and_(a, or_(a, b)), a
                ));

                // left identity
                BOOST_HANA_CHECK(equal(
                    or_(f, a), a
                ));
                BOOST_HANA_CHECK(equal(
                    and_(t, a), a
                ));

                // distributivity
                BOOST_HANA_CHECK(equal(
                    or_(a, and_(b, c)), and_(or_(a, b), or_(a, c))
                ));
                BOOST_HANA_CHECK(equal(
                    and_(a, or_(b, c)), or_(and_(a, b), and_(a, c))
                ));

                // complements
                BOOST_HANA_CHECK(is_true_valued(or_(a, not_(a))));
                BOOST_HANA_CHECK(is_false_valued(and_(a, not_(a))));
            });});});});});
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_LOGICAL_HPP
