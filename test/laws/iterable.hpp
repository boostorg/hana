/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_ITERABLE_HPP
#define BOOST_HANA_TEST_LAWS_ITERABLE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/capture.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/range.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename It, typename = when<true>>
    struct TestIterable : TestIterable<It, laws> {
        using TestIterable<It, laws>::TestIterable;
    };

    template <typename It>
    struct TestIterable<It, laws> {
        template <typename Xs>
        TestIterable(Xs xs) {
            hana::for_each(xs, [](auto xs) {
                static_assert(Iterable<decltype(xs)>::value, "");

                BOOST_HANA_CONSTANT_CHECK(
                    hana::is_empty(xs) ^iff^ hana::is_empty(hana::to<tuple_tag>(xs))
                );

                only_when_(hana::not_(hana::is_empty(xs)), hana::make_lazy([](auto xs) {
                    BOOST_HANA_CHECK(hana::equal(
                        hana::front(xs),
                        hana::front(hana::to<tuple_tag>(xs))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::to<tuple_tag>(hana::tail(xs)),
                        hana::tail(hana::to<tuple_tag>(xs))
                    ));

                    // methods
                    // drop_front(xs, 1) == tail(xs) unless xs is empty
                    BOOST_HANA_CHECK(hana::equal(
                        hana::drop_front(xs, size_c<1>),
                        hana::tail(xs)
                    ));

                    // back(xs) == at(xs, length(xs)-1)
                    BOOST_HANA_CHECK(hana::equal(
                        hana::back(xs),
                        hana::at(xs, hana::minus(hana::length(xs), hana::size_c<1>))
                    ));

                })(xs));

                // drop_front(xs, 0) == xs
                BOOST_HANA_CHECK(hana::equal(
                    hana::drop_front(xs, size_c<0>),
                    xs
                ));

                // at(xs, n) == front(drop_front(xs, n))
                hana::for_each(hana::make_range(size_c<0>, hana::length(xs)),
                hana::capture(xs)([](auto xs, auto n) {
                    BOOST_HANA_CHECK(hana::equal(
                        hana::at(xs, n),
                        hana::front(hana::drop_front(xs, n))
                    ));
                }));

                // Searchable
                hana::eval_if(hana::is_empty(xs),
                    hana::make_lazy([](auto xs) {
                        BOOST_HANA_CONSTANT_CHECK(
                            hana::not_(hana::any_of(xs, hana::always(true_c)))
                        );

                        BOOST_HANA_CONSTANT_CHECK(hana::equal(
                            hana::find_if(xs, hana::always(true_c)),
                            nothing
                        ));
                    })(xs),
                    hana::make_lazy([](auto xs) {
                        BOOST_HANA_CHECK(
                            hana::any_of(xs, hana::always(true_c))
                        );
                        BOOST_HANA_CHECK(
                            hana::not_(hana::any_of(xs, hana::always(false_c)))
                        );

                        BOOST_HANA_CHECK(hana::equal(
                            hana::find_if(xs, hana::always(true_c)),
                            hana::just(hana::front(xs))
                        ));
                    })(xs)
                );

            });
        }
    };

    template <typename S>
    struct TestIterable<S, when<Sequence<S>::value>>
        : TestIterable<S, laws>
    {
        template <int i>
        using x = ct_eq<i>;

        template <int i = 0>
        struct invalid { };

        struct undefined { };

        template <typename Xs>
        TestIterable(Xs xs) : TestIterable<S, laws>{xs} {
            constexpr auto list = make<S>;

            //////////////////////////////////////////////////////////////////
            // front
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                front(list(x<0>{})),
                x<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                front(list(x<0>{}, invalid<>{})),
                x<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                front(list(x<0>{}, invalid<1>{}, invalid<2>{})),
                x<0>{}
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                front(list(1)), 1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                front(list(1, '2')), 1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                front(list(1, '2', 3.3)), 1
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
                at(list(x<0>{}), size_c<0>),
                x<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(list(x<0>{}, invalid<>{}), size_c<0>),
                x<0>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(list(invalid<>{}, x<1>{}), size_c<1>),
                x<1>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(list(invalid<0>{}, x<1>{}, invalid<2>{}), size_c<1>),
                x<1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(list(invalid<0>{}, invalid<1>{}, x<2>{}), size_c<2>),
                x<2>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(list(invalid<0>{}, invalid<1>{}, x<2>{}, invalid<3>{}), size_c<2>),
                x<2>{}
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(list(1), size_c<0>),
                1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(list(1, '2'), size_c<0>),
                1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(list(1, '2', 3.3), size_c<0>),
                1
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(list(1, '2'), size_c<1>),
                '2'
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(list(1, '2', 3.3), size_c<1>),
                '2'
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                at(list(1, '2', 3.3), size_c<2>),
                3.3
            ));

            // make sure we can use non-pods on both sides
            at(list(Tracked{0}, x<1>{}, Tracked{1}), size_c<1>);


            //////////////////////////////////////////////////////////////////
            // back
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                back(list(x<0>{})),
                x<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                back(list(invalid<0>{}, x<1>{})),
                x<1>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                back(list(invalid<0>{}, invalid<1>{}, x<2>{})),
                x<2>{}
            ));

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                back(list(1)), 1
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                back(list(1, '2')), '2'
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                back(list(1, '2', 3.3)), 3.3
            ));


            //////////////////////////////////////////////////////////////////
            // drop_front
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(), size_c<0>),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(), size_c<1>),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(), size_c<2>),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(x<0>{}), size_c<0>),
                list(x<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(x<0>{}), size_c<1>),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(x<0>{}), size_c<2>),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(x<0>{}, x<1>{}), size_c<0>),
                list(x<0>{}, x<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(x<0>{}, x<1>{}), size_c<1>),
                list(x<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front(list(x<0>{}, x<1>{}), size_c<2>),
                list()
            ));

            // make sure drop_front(xs) == drop_front(xs, size_c<1>)
            BOOST_HANA_CHECK(equal(
                drop_front(list()),
                drop_front(list(), size_c<1>)
            ));

            BOOST_HANA_CHECK(equal(
                drop_front(list(x<0>{})),
                drop_front(list(x<0>{}), size_c<1>)
            ));

            BOOST_HANA_CHECK(equal(
                drop_front(list(x<0>{}, x<1>{})),
                drop_front(list(x<0>{}, x<1>{}), size_c<1>)
            ));

            BOOST_HANA_CHECK(equal(
                drop_front(list(x<0>{}, x<2>{})),
                drop_front(list(x<0>{}, x<2>{}), size_c<1>)
            ));

            //////////////////////////////////////////////////////////////////
            // drop_front_exactly
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front_exactly(list(), size_c<0>),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front_exactly(list(x<0>{}), size_c<0>),
                list(x<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front_exactly(list(x<0>{}), size_c<1>),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front_exactly(list(x<0>{}, x<1>{}), size_c<0>),
                list(x<0>{}, x<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front_exactly(list(x<0>{}, x<1>{}), size_c<1>),
                list(x<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front_exactly(list(x<0>{}, x<1>{}), size_c<2>),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_front_exactly(
                    list(x<0>{}, x<1>{}, x<2>{}, x<3>{},
                         x<4>{}, x<5>{}, x<6>{}, x<7>{}), size_c<4>),
                list(x<4>{}, x<5>{}, x<6>{}, x<7>{})
            ));

            // make sure drop_front_exactly(xs) == drop_front_exactly(xs, size_c<1>)
            BOOST_HANA_CHECK(equal(
                drop_front_exactly(list(x<0>{})),
                drop_front_exactly(list(x<0>{}), size_c<1>)
            ));

            BOOST_HANA_CHECK(equal(
                drop_front_exactly(list(x<0>{}, x<1>{})),
                drop_front_exactly(list(x<0>{}, x<1>{}), size_c<1>)
            ));

            BOOST_HANA_CHECK(equal(
                drop_front_exactly(list(x<0>{}, x<2>{})),
                drop_front_exactly(list(x<0>{}, x<2>{}), size_c<1>)
            ));

            //////////////////////////////////////////////////////////////////
            // drop_while
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(), id),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(true_c), id),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(false_c), id),
                list(false_c)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(true_c, true_c), id),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(true_c, false_c), id),
                list(false_c)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(false_c, true_c), id),
                list(false_c, true_c)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(false_c, false_c), id),
                list(false_c, false_c)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(x<0>{}, x<1>{}), not_equal.to(x<99>{})),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(x<0>{}, x<1>{}, x<2>{}), not_equal.to(x<1>{})),
                list(x<1>{}, x<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(x<0>{}, x<1>{}, x<2>{}, x<3>{}), not_equal.to(x<3>{})),
                list(x<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                drop_while(list(x<0>{}, x<1>{}, x<2>{}, x<3>{}), not_equal.to(x<0>{})),
                list(x<0>{}, x<1>{}, x<2>{}, x<3>{})
            ));

            //////////////////////////////////////////////////////////////////
            // lexicographical_compare
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(),
                list()
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::lexicographical_compare(
                list(),
                list(undefined{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(undefined{}),
                list()
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(ct_ord<0>{}),
                list(ct_ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::lexicographical_compare(
                list(ct_ord<0>{}),
                list(ct_ord<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(ct_ord<1>{}),
                list(ct_ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(ct_ord<0>{}, undefined{}),
                list(ct_ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::lexicographical_compare(
                list(ct_ord<0>{}),
                list(ct_ord<0>{}, undefined{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<0>{}),
                list(ct_ord<0>{}, ct_ord<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<1>{}),
                list(ct_ord<0>{}, ct_ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}),
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}, undefined{}),
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}),
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}, undefined{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<3>{}, undefined{}),
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}, undefined{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}, undefined{}),
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<3>{}, undefined{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}, undefined{}),
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<3>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::lexicographical_compare(
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<3>{}),
                list(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}, undefined{})
            )));
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_ITERABLE_HPP
