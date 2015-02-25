/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_ORDERABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_ORDERABLE_HPP

#include <boost/hana/orderable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/logical.hpp>

#include <test/auto/base.hpp>
#include <test/auto/comparable.hpp>


namespace boost { namespace hana { namespace test {
    template <typename Ord>
    auto laws<Orderable, Ord> = [] {
        static_assert(models<Orderable(Ord)>{}, "");

        laws<Comparable, Ord>();

        for_each(objects<Ord>, [=](auto a) {
        for_each(objects<Ord>, [=](auto b) {
        for_each(objects<Ord>, [=](auto c) {

            BOOST_HANA_CHECK(
                and_(less_equal(a, b), less_equal(b, a))
                            ^implies^
                            equal(a, b)
            );

            BOOST_HANA_CHECK(
                and_(less_equal(a, b), less_equal(b, c))
                            ^implies^
                        less_equal(a, c)
            );

            BOOST_HANA_CHECK(
                or_(less_equal(a, b), less_equal(b, a))
            );


            BOOST_HANA_CHECK(
                less(a, b) ^iff^ not_(less_equal(b, a))
            );

            BOOST_HANA_CHECK(
                greater(a, b) ^iff^ less(b, a)
            );

            BOOST_HANA_CHECK(
                greater_equal(a, b) ^iff^ not_(less(a, b))
            );

            // less.than & al.
            BOOST_HANA_CHECK(less.than(a)(b) ^iff^ less(b, a));
            BOOST_HANA_CHECK(greater.than(a)(b) ^iff^ greater(b, a));
            BOOST_HANA_CHECK(less_equal.than(a)(b) ^iff^ less_equal(b, a));
            BOOST_HANA_CHECK(greater_equal.than(a)(b) ^iff^ greater_equal(b, a));

        });});});
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_ORDERABLE_HPP
