/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_COMPARABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_COMPARABLE_HPP

#include <boost/hana/comparable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/logical.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename T>
    auto laws<Comparable, T> = [] {
        static_assert(models<Comparable(T)>{}, "");

        for_each(objects<T>, [](auto a) {
            BOOST_HANA_CHECK(equal(a, a));

            for_each(objects<T>, [=](auto b) {
                BOOST_HANA_CHECK(
                    not_equal(a, b) ^iff^ not_(equal(a, b))
                );

                BOOST_HANA_CHECK(
                    equal(a, b) ^implies^ equal(b, a)
                );

                for_each(objects<T>, [=](auto c) {
                    BOOST_HANA_CHECK(
                        and_(equal(a, b), equal(b, c)) ^implies^ equal(a, c)
                    );
                });

                // operators
                eval_if(bool_<has_operator<T, decltype(equal)>{}>,
                    [=](auto _) {
                        BOOST_HANA_CHECK(
                            equal(a, b) ^iff^ (_(a) == _(b))
                        );
                    },
                    [](auto _) { }
                );

                eval_if(bool_<has_operator<T, decltype(not_equal)>{}>,
                    [=](auto _) {
                        BOOST_HANA_CHECK(
                            not_equal(a, b) ^iff^ (_(a) != _(b))
                        );
                    },
                    [](auto _) { }
                );

                // equal.to and not_equal.to
                BOOST_HANA_CHECK(
                    equal.to(a)(b) ^iff^ equal(a, b)
                );

                BOOST_HANA_CHECK(
                    not_equal.to(a)(b) ^iff^ not_equal(a, b)
                );
            });
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_COMPARABLE_HPP
