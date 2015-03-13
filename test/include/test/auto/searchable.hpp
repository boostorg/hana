/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_SEARCHABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_SEARCHABLE_HPP

#include <boost/hana/searchable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename S>
    auto laws<Searchable, S> = [] {
        static_assert(_models<Searchable, S>{}, "");

        for_each(objects<S>, [](auto xs) {

            auto predicates = make<Tuple>(always(true_), always(false_));
            for_each(predicates, [=](auto p) {
                BOOST_HANA_CHECK(
                    any_of(xs, p) ^iff^ not_(all_of(xs, compose(not_, p)))
                );

                BOOST_HANA_CHECK(
                    any_of(xs, p) ^iff^ not_(none_of(xs, p))
                );
            });

            eval_if(all_of(xs, models<Logical>),
                [=](auto _) {
                    BOOST_HANA_CHECK(
                        _(any)(xs) ^iff^ _(any_of)(xs, id)
                    );

                    BOOST_HANA_CHECK(
                        _(all)(xs) ^iff^ _(all_of)(xs, id)
                    );

                    BOOST_HANA_CHECK(
                        _(none)(xs) ^iff^ _(none_of)(xs, id)
                    );
                },
                [](auto) { }
            );

            // If there's at least one element, we find it and then perform
            // the check for `find` with it.
            maybe(0,
                [=](auto x) {
                    //! @todo
                    //! `find` will only work on values that can be compared
                    //! at compile-time, because it has to return a `nothing`
                    //! or a `just(x)`. Because of this, if the Searchable
                    //! contains elements that must be compared at runtime,
                    //! using `find` would fail. We workaround this by
                    //! not checking the law when this is the case, but
                    //! that's a hack.
                    auto compile_time_comparable = [=](auto y) {
                        return models<Constant, decltype(equal(x, y))>;
                    };
                    eval_if(all_of(xs, compile_time_comparable),
                        [=](auto _) {
                            BOOST_HANA_CHECK(equal(
                                _(find)(xs, x),
                                _(find_if)(xs, [=](auto y) { return equal(y, x); })
                            ));
                        },
                        [](auto) { }
                    );
                },
                find_if(xs, always(true_))
            );

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(xs, always(false_)),
                nothing
            ));

            for_each(objects<S>, [=](auto ys) {
                BOOST_HANA_CHECK(
                    subset(xs, ys) ^iff^ all_of(xs, [=](auto x) {
                        return elem(ys, x);
                    })
                );
            });
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_SEARCHABLE_HPP
