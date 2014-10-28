/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_ITERABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_ITERABLE_HPP

#include <boost/hana/iterable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/tuple.hpp>

// provided instances
#include <test/auto/foldable.hpp>
#include <test/auto/searchable.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>


namespace boost { namespace hana { namespace test {
    template <typename It>
    auto laws<Iterable, It> = [] {
        // Instance-wide laws
        {
            for_each(objects<It>, [](auto xs) {
                BOOST_HANA_CONSTANT_ASSERT(
                    is_empty(xs) ^iff^ is_empty(to<Tuple>(xs))
                );

                eval_if(is_empty(xs), [](auto ) {}, [=](auto _) {
                    BOOST_HANA_ASSERT(equal(
                        _(head)(xs),
                        head(_(to<Tuple>)(xs))
                    ));

                    BOOST_HANA_ASSERT(equal(
                        to<Tuple>(_(tail)(xs)),
                        tail(_(to<Tuple>)(xs))
                    ));
                });
            });
        }

        // Iterable methods
        {
            for_each(objects<It>, [](auto iterable) {
                // drop(0, xs) == xs
                BOOST_HANA_ASSERT(equal(
                    drop(int_<0>, iterable),
                    iterable
                ));

                // drop(1, xs) == tail(xs) unless xs is empty
                eval_if(is_empty(iterable), [](auto) { }, [=](auto _) {
                    BOOST_HANA_ASSERT(equal(
                        _(drop)(int_<1>, iterable),
                        _(tail)(iterable)
                    ));
                });

                // at(n, xs) == head(drop(n, xs))
                for_each(range(int_<0>, length(iterable)), [=](auto n) {
                    BOOST_HANA_ASSERT(equal(
                        at(n, iterable),
                        head(drop(n, iterable))
                    ));
                });

                // last(xs) == at(length(xs)-1, xs)
                eval_if(is_empty(iterable), [](auto) {}, [=](auto _) {
                    BOOST_HANA_ASSERT(equal(
                        _(last)(iterable),
                        at(pred(_(length)(iterable)), iterable)
                    ));
                });
            });
        }

        // Foldable instance
        {
            laws<Foldable, It>();

            auto state = test::injection([]{})();
            auto f = test::injection([]{});

            auto check = map(
                // when length == 0
                pair(int_<0>, [=](auto xs) {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        foldl(xs, state, f),
                        state
                    ));

                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        foldr(xs, state, f),
                        state
                    ));
                }),

                // when length == 1
                pair(int_<1>, [=](auto xs) {
                    BOOST_HANA_ASSERT(equal(
                        foldl(xs, state, f),
                        f(state, at(int_<0>, xs))
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldr(xs, state, f),
                        f(at(int_<0>, xs), state)
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldl1(xs, f),
                        at(int_<0>, xs)
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldr1(xs, f),
                        at(int_<0>, xs)
                    ));
                }),

                // when length == 2
                pair(int_<2>, [=](auto xs) {
                    BOOST_HANA_ASSERT(equal(
                        foldl(xs, state, f),
                        f(f(state, at(int_<0>, xs)), at(int_<1>, xs))
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldr(xs, state, f),
                        f(at(int_<0>, xs), f(at(int_<1>, xs), state))
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldl1(xs, f),
                        f(at(int_<0>, xs), at(int_<1>, xs))
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldr1(xs, f),
                        f(at(int_<0>, xs), at(int_<1>, xs))
                    ));
                }),

                // when length == 3
                pair(int_<3>, [=](auto xs) {
                    BOOST_HANA_ASSERT(equal(
                        foldl(xs, state, f),
                        f(f(f(state, at(int_<0>, xs)), at(int_<1>, xs)), at(int_<2>, xs))
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldr(xs, state, f),
                        f(at(int_<0>, xs), f(at(int_<1>, xs), f(at(int_<2>, xs), state)))
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldl1(xs, f),
                        f(f(at(int_<0>, xs), at(int_<1>, xs)), at(int_<2>, xs))
                    ));

                    BOOST_HANA_ASSERT(equal(
                        foldr1(xs, f),
                        f(at(int_<0>, xs), f(at(int_<1>, xs), at(int_<2>, xs)))
                    ));
                })

                // etc...
            );

            for_each(objects<It>, [=](auto xs) {
                maybe(0, // do nothing if we have no test for that length
                    [=](auto test) { test(xs); },
                    lookup(check, length(xs))
                );
            });
        }

        // Searchable instance
        {
            laws<Searchable, It>();

            for_each(objects<It>, [=](auto xs) {
                eval_if(is_empty(xs),
                    [=](auto _) {
                        BOOST_HANA_CONSTANT_ASSERT(not_(_(any)(xs, always(true_))));

                        BOOST_HANA_CONSTANT_ASSERT(equal(
                            _(find)(xs, always(true_)),
                            nothing
                        ));
                    },
                    [=](auto _) {
                        BOOST_HANA_ASSERT(_(any)(xs, always(true_)));
                        BOOST_HANA_ASSERT(not_(_(any)(xs, always(false_))));

                        BOOST_HANA_ASSERT(equal(
                            _(find)(xs, always(true_)),
                            just(_(head)(xs))
                        ));

                        BOOST_HANA_ASSERT(equal(
                            _(find)(xs, always(false_)),
                            nothing
                        ));
                    }
                );
            });
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_ITERABLE_HPP
