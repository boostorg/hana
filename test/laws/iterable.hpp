/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_ITERABLE_HPP
#define BOOST_HANA_TEST_LAWS_ITERABLE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/range.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename It, typename = when<true>>
    struct TestIterable : TestIterable<It, laws> {
        using TestIterable<It, laws>::TestIterable;
    };

    template <typename It>
    struct TestIterable<It, laws> {
        static_assert(models<Iterable, It>{}, "");

        template <typename Xs>
        TestIterable(Xs xs) {
            hana::for_each(xs, [](auto xs) {

                BOOST_HANA_CONSTANT_CHECK(
                    hana::is_empty(xs) ^iff^ hana::is_empty(hana::to<Tuple>(xs))
                );

                only_when_(hana::not_(hana::is_empty(xs)), [=](auto _) {
                    auto xs_ = _(xs);

                    BOOST_HANA_CHECK(hana::equal(
                        hana::head(xs_),
                        hana::head(hana::to<Tuple>(xs_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::to<Tuple>(hana::tail(xs_)),
                        hana::tail(hana::to<Tuple>(xs_))
                    ));

                    // methods
                    // drop(1, xs) == tail(xs) unless xs is empty
                    BOOST_HANA_CHECK(hana::equal(
                        hana::drop(size_t<1>, xs_),
                        hana::tail(xs_)
                    ));

                    // last(xs) == at(length(xs)-1, xs)
                    BOOST_HANA_CHECK(hana::equal(
                        hana::last(xs_),
                        hana::at(hana::pred(hana::length(xs_)), xs_)
                    ));

                });

                // drop(0, xs) == xs
                BOOST_HANA_CHECK(hana::equal(
                    hana::drop(size_t<0>, xs),
                    xs
                ));

                // at(n, xs) == head(drop(n, xs))
                hana::for_each(hana::make_range(size_t<0>, hana::length(xs)), [=](auto n) {
                    BOOST_HANA_CHECK(hana::equal(
                        hana::at(n, xs),
                        hana::head(hana::drop(n, xs))
                    ));
                });

                // operators
                only_when_(bool_<has_operator<It, decltype(at)>{}>, [=](auto _) {
                    hana::for_each(hana::make_range(size_t<0>, hana::length(xs)), [=](auto n) {
                        BOOST_HANA_CHECK(hana::equal(
                            hana::at(n, _(xs)),
                            _(xs)[n]
                        ));
                    });
                });

                // Foldable
                // when length == 0
                test::_injection<0> f{};
                test::_constant<0> state{};

                only_when_(hana::equal(hana::length(xs), size_t<0>), [=](auto _) {
                    auto xs_ = _(xs);
                    BOOST_HANA_CONSTANT_CHECK(hana::equal(
                        hana::foldl(xs_, state, f),
                        state
                    ));

                    BOOST_HANA_CONSTANT_CHECK(hana::equal(
                        hana::foldr(xs_, state, f),
                        state
                    ));
                });

                // when length == 1
                only_when_(hana::equal(hana::length(xs), size_t<1>), [=](auto _) {
                    auto xs_ = _(xs);

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldl(xs_, state, f),
                        f(state, at_c<0>(xs_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldr(xs_, state, f),
                        f(at_c<0>(xs_), state)
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldl1(xs_, f),
                        at_c<0>(xs_)
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldr1(xs_, f),
                        at_c<0>(xs_)
                    ));
                });

                // when length == 2
                only_when_(hana::equal(hana::length(xs), size_t<2>), [=](auto _) {
                    auto xs_ = _(xs);

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldl(xs_, state, f),
                        f(f(state, at_c<0>(xs_)), at_c<1>(xs_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldr(xs_, state, f),
                        f(at_c<0>(xs_), f(at_c<1>(xs_), state))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldl1(xs_, f),
                        f(at_c<0>(xs_), at_c<1>(xs_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldr1(xs_, f),
                        f(at_c<0>(xs_), at_c<1>(xs_))
                    ));
                });

                // when length == 3
                only_when_(hana::equal(hana::length(xs), size_t<3>), [=](auto _) {
                    auto xs_ = _(xs);

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldl(xs_, state, f),
                        f(f(f(state, at_c<0>(xs_)), at_c<1>(xs_)), at_c<2>(xs_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldr(xs_, state, f),
                        f(at_c<0>(xs_), f(at_c<1>(xs_), f(at_c<2>(xs_), state)))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldl1(xs_, f),
                        f(f(at_c<0>(xs_), at_c<1>(xs_)), at_c<2>(xs_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldr1(xs_, f),
                        f(at_c<0>(xs_), f(at_c<1>(xs_), at_c<2>(xs_)))
                    ));
                });

                // when length == 4
                only_when_(hana::equal(hana::length(xs), size_t<4>), [=](auto _) {
                    auto xs_ = _(xs);

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldl(xs_, state, f),
                        f(f(f(f(state, at_c<0>(xs_)), at_c<1>(xs_)), at_c<2>(xs_)), at_c<3>(xs_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldr(xs_, state, f),
                        f(at_c<0>(xs_), f(at_c<1>(xs_), f(at_c<2>(xs_), f(at_c<3>(xs_), state))))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldl1(xs_, f),
                        f(f(f(at_c<0>(xs_), at_c<1>(xs_)), at_c<2>(xs_)), at_c<3>(xs_))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::foldr1(xs_, f),
                        f(at_c<0>(xs_), f(at_c<1>(xs_), f(at_c<2>(xs_), at_c<3>(xs_))))
                    ));
                });

                // Searchable
                hana::eval_if(hana::is_empty(xs),
                    [=](auto _) {
                        auto xs_ = _(xs);

                        BOOST_HANA_CONSTANT_CHECK(
                            hana::not_(hana::any_of(xs_, hana::always(true_)))
                        );

                        BOOST_HANA_CONSTANT_CHECK(hana::equal(
                            hana::find(xs_, hana::always(true_)),
                            nothing
                        ));
                    },
                    [=](auto _) {
                        auto xs_ = _(xs);

                        BOOST_HANA_CHECK(
                            hana::any_of(xs_, hana::always(true_))
                        );
                        BOOST_HANA_CHECK(
                            hana::not_(hana::any_of(xs_, hana::always(false_)))
                        );

                        BOOST_HANA_CHECK(hana::equal(
                            hana::find(xs_, hana::always(true_)),
                            hana::just(hana::head(xs_))
                        ));
                    }
                );

            });
        }
    };

    template <typename S>
    struct TestIterable<S, when<models<Sequence, S>{}>> : TestIterable<S, laws> {
        template <int i>
        using x = _constant<i>;

        template <int i = 0>
        struct invalid { };

        template <typename Xs>
        TestIterable(Xs xs) : TestIterable<S, laws>{xs} {
            constexpr auto list = make<S>;

            //////////////////////////////////////////////////////////////////
            // head
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(list(x<0>{})),
                x<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(list(x<0>{}, invalid<>{})),
                x<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(list(x<0>{}, invalid<1>{}, invalid<2>{})),
                x<0>{}
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                head(list(1)), 1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                head(list(1, '2')), 1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                head(list(1, '2', 3.3)), 1
            ));

            //////////////////////////////////////////////////////////////////
            // is_empty
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(is_empty(list()));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(list(invalid<>{}))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(list(invalid<0>{}, invalid<1>{}))));

            int i = 0; // non-constexpr
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(list(i))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(list(i, i))));

            //////////////////////////////////////////////////////////////////
            // tail
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(list(invalid<>{}, x<0>{})), list(x<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(list(invalid<>{}, x<0>{}, x<1>{})), list(x<0>{}, x<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(list(invalid<>{}, x<0>{}, x<1>{}, x<2>{})), list(x<0>{}, x<1>{}, x<2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(list(1)), list()
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                tail(list(1, '2')), list('2')
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                tail(list(1, '2', 3.3)), list('2', 3.3)
            ));


            //////////////////////////////////////////////////////////////////
            // at
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(size_t<0>, list(x<0>{})), x<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(size_t<0>, list(x<0>{}, invalid<>{})), x<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(size_t<1>, list(invalid<>{}, x<1>{})), x<1>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(size_t<1>, list(invalid<0>{}, x<1>{}, invalid<2>{})), x<1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(size_t<2>, list(invalid<0>{}, invalid<1>{}, x<2>{})), x<2>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(size_t<2>, list(invalid<0>{}, invalid<1>{}, x<2>{}, invalid<3>{})), x<2>{}
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(size_t<0>, list(1)), 1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(size_t<0>, list(1, '2')), 1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(size_t<0>, list(1, '2', 3.3)), 1
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(size_t<1>, list(1, '2')), '2'
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(size_t<1>, list(1, '2', 3.3)), '2'
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(size_t<2>, list(1, '2', 3.3)), 3.3
            ));

            // make sure we can use non-pods on both sides
            at(size_t<1>, list(Tracked{0}, x<1>{}, Tracked{1}));


            //////////////////////////////////////////////////////////////////
            // last
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(list(x<0>{})),
                x<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(list(invalid<0>{}, x<1>{})),
                x<1>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                last(list(invalid<0>{}, invalid<1>{}, x<2>{})),
                x<2>{}
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                last(list(1)), 1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                last(list(1, '2')), '2'
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                last(list(1, '2', 3.3)), 3.3
            ));


            //////////////////////////////////////////////////////////////////
            // drop
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<0>, list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<1>, list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<2>, list()),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<0>, list(x<0>{})),
                list(x<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<1>, list(x<0>{})),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<2>, list(x<0>{})),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<0>, list(x<0>{}, x<1>{})),
                list(x<0>{}, x<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<1>, list(x<0>{}, x<1>{})),
                list(x<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop(size_t<2>, list(x<0>{}, x<1>{})),
                list()
            ));

            //////////////////////////////////////////////////////////////////
            // drop_until
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_until(list(), id),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_until(list(true_), id),
                list(true_)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_until(list(false_), id),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_until(list(true_, true_), id),
                list(true_, true_)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_until(list(true_, false_), id),
                list(true_, false_)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_until(list(false_, true_), id),
                list(true_)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_until(list(false_, false_), id),
                list()
            ));

            //////////////////////////////////////////////////////////////////
            // drop_while
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(), id),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(true_), id),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(false_), id),
                list(false_)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(true_, true_), id),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(true_, false_), id),
                list(false_)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(false_, true_), id),
                list(false_, true_)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(false_, false_), id),
                list(false_, false_)
            ));
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_ITERABLE_HPP
