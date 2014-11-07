/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_LIST_HPP
#define BOOST_HANA_TEST_TEST_AUTO_LIST_HPP

#include <boost/hana/list.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>

// provided instances
#include <test/auto/applicative.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/functor.hpp>
#include <test/auto/monad.hpp>
#include <test/auto/searchable.hpp>
#include <test/auto/traversable.hpp>

#include <test/auto/base.hpp>
#include <test/cnumeric.hpp>
#include <test/equivalence_class.hpp>
#include <test/identity.hpp>
#include <test/injection.hpp>
#include <test/minimal_product.hpp>
#include <test/numeric.hpp>
#include <test/seq.hpp>
#include <vector>


namespace boost { namespace hana { namespace test {
    namespace list_detail {
        template <int i>
        struct _non_pod { virtual ~_non_pod() { } };

        template <int i = 0>
        _non_pod<i> non_pod{};


        template <int i>
        struct _invalid { };

        template <int i = 0>
        constexpr _invalid<i> invalid{};

        template <int i>
        constexpr decltype(cnumeric<int, i>) ord = cnumeric<int, i>;
    }

    template <typename L>
    auto laws<List, L> = [] {
        auto list = make<L>;
        auto foldable = seq;
        using list_detail::ord;
        using list_detail::invalid;
        constexpr struct { } undefined{};

        // Instance-wide laws
        {
            for_each(objects<L>, [](auto xs) {
                for_each(objects<L>, [=](auto ys) {
                    BOOST_HANA_ASSERT(
                        equal(xs, ys) ^iff^ equal(to<Tuple>(xs), to<Tuple>(ys))
                    );
                });
            });
        }

        // Conversions
        {
            // Foldable -> List
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<L>(foldable()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<L>(foldable(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<L>(foldable(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<L>(foldable(x<0>, x<1>, x<2>)),
                    list(x<0>, x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<L>(foldable(x<0>, x<1>, x<2>, x<3>)),
                    list(x<0>, x<1>, x<2>, x<3>)
                ));
            }
        }

        // Instance methods
        {
            constexpr struct { } undefined{};
            auto prod = test::minimal_product;
            auto is = [](auto x) {
                return [=](auto y) { return equal(x, y); };
            };

            auto isnt = [](auto x) {
                return [=](auto y) { return not_equal(x, y); };
            };


            // make (that's tautological given our definition of `list`, but w/e)
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    make<L>(),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    make<L>(x<0>),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    make<L>(x<0>, x<1>),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    make<L>(x<0>, x<1>, x<2>),
                    list(x<0>, x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    make<L>(x<0>, x<1>, x<2>, x<3>),
                    list(x<0>, x<1>, x<2>, x<3>)
                ));
            }

            // cons
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    cons(x<0>, list()),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    cons(x<0>, list(x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    cons(x<0>, list(x<1>, x<2>)),
                    list(x<0>, x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    cons(x<0>, list(x<1>, x<2>, x<3>)),
                    list(x<0>, x<1>, x<2>, x<3>)
                ));
            }

            // nil
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    nil<L>, list()
                ));
            }

            // concat
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(), list(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(), list(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(x<0>), list()),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(x<0>), list(x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(x<0>), list(x<1>, x<2>)),
                    list(x<0>, x<1>, x<2>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(x<0>, x<1>), list()),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(x<0>, x<1>), list(x<2>)),
                    list(x<0>, x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    concat(list(x<0>, x<1>), list(x<2>, x<3>)),
                    list(x<0>, x<1>, x<2>, x<3>)
                ));
            }

            // init
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    init(list(undefined)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    init(list(x<0>, undefined)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    init(list(x<0>, x<1>, undefined)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    init(list(x<0>, x<1>, x<2>, undefined)),
                    list(x<0>, x<1>, x<2>)
                ));
            }

            // intersperse
            {
                auto z = x<999>;
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    intersperse(list(), undefined),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    intersperse(list(x<0>), undefined),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    intersperse(list(x<0>, x<1>), z),
                    list(x<0>, z, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    intersperse(list(x<0>, x<1>, x<2>), z),
                    list(x<0>, z, x<1>, z, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    intersperse(list(x<0>, x<1>, x<2>, x<3>), z),
                    list(x<0>, z, x<1>, z, x<2>, z, x<3>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    intersperse(list(x<0>, x<1>, x<2>, x<3>, x<4>), z),
                    list(x<0>, z, x<1>, z, x<2>, z, x<3>, z, x<4>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    intersperse(list(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>), z),
                    list(x<0>, z, x<1>, z, x<2>, z, x<3>, z, x<4>, z, x<5>)
                ));
            }

            // slice
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(), int_<0>, int_<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(undefined), int_<0>, int_<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(undefined, undefined), int_<0>, int_<0>),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(x<0>), int_<0>, int_<1>),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(x<0>, undefined), int_<0>, int_<1>),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(undefined, x<1>), int_<1>, int_<2>),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(undefined, x<1>, undefined), int_<1>, int_<2>),
                    list(x<1>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(x<0>, x<1>), int_<0>, int_<2>),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(x<0>, x<1>, undefined), int_<0>, int_<2>),
                    list(x<0>, x<1>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    slice(list(undefined, x<1>, x<2>), int_<1>, int_<3>),
                    list(x<1>, x<2>)
                ));
            }

            // snoc
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    snoc(list(), x<0>),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    snoc(list(x<0>), x<1>),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    snoc(list(x<0>, x<1>), x<2>),
                    list(x<0>, x<1>, x<2>)
                ));
            }

            // take
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<0>, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<1>, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<2>, list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<0>, list(x<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<1>, list(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<2>, list(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<3>, list(x<0>)),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<0>, list(x<0>, x<1>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<1>, list(x<0>, x<1>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<2>, list(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<3>, list(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take(int_<10>, list(x<0>, x<1>, x<2>, x<3>,  x<4>,  x<5>,  x<6>,
                                        x<7>, x<8>, x<9>, x<10>, x<11>, x<12>, x<13>)),
                    list(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>)
                ));
            }

            // repeat
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    repeat<L>(int_<0>, x<0>),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    repeat<L>(int_<1>, x<0>),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    repeat<L>(int_<2>, x<0>),
                    list(x<0>, x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    repeat<L>(int_<3>, x<0>),
                    list(x<0>, x<0>, x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    repeat<L>(int_<4>, x<0>),
                    list(x<0>, x<0>, x<0>, x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    repeat<L>(int_<5>, x<0>),
                    list(x<0>, x<0>, x<0>, x<0>, x<0>)
                ));
            }

            // reverse
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    reverse(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    reverse(list(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    reverse(list(x<0>, x<1>)),
                    list(x<1>, x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    reverse(list(x<0>, x<1>, x<2>)),
                    list(x<2>, x<1>, x<0>)
                ));
            }

            // sort
            {

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort(list(ord<0>)),
                    list(ord<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort(list(ord<0>, ord<1>)),
                    list(ord<0>, ord<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort(list(ord<1>, ord<0>)),
                    list(ord<0>, ord<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort(list(ord<1>, ord<0>, ord<4>, ord<2>)),
                    list(ord<0>, ord<1>, ord<2>, ord<4>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort(list(ord<1>, ord<0>, ord<-4>, ord<2>)),
                    list(ord<-4>, ord<0>, ord<1>, ord<2>)
                ));
            }

            // unzip
            {
                auto t = list; // tests are unreadable otherwise. mnemonic: tuple

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t())),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(), t())),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(x<0>, x<2>), t(x<1>, x<3>, x<4>))),
                    list(t(x<0>, x<1>), t(x<2>, x<3>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(),     t(),     t()))    ,
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(x<0>), t(),     t()))    ,
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(),     t(x<1>), t()))    ,
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(),     t(),     t(x<2>))),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(x<0>), t(x<1>), t()))    ,
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(),     t(x<1>), t(x<2>))),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(x<0>), t(),     t(x<2>))),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(x<0>), t(x<1>), t(x<2>))),
                    list(t(x<0>, x<1>, x<2>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unzip(list(t(x<0>, x<3>), t(x<1>, x<4>), t(x<2>, x<5>))),
                    list(t(x<0>, x<1>, x<2>), t(x<3>, x<4>, x<5>))
                ));
            }

            // span
            {
                auto z = x<999>;

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(), is(z)),
                    prod(list(), list())
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(x<0>), is(z)),
                    prod(list(), list(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(z), is(z)),
                    prod(list(z), list())
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(x<0>, z), is(z)),
                    prod(list(), list(x<0>, z))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(z, x<0>), is(z)),
                    prod(list(z), list(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(x<0>, x<1>), is(z)),
                    prod(list(), list(x<0>, x<1>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(x<0>, x<1>, x<2>), is(z)),
                    prod(list(), list(x<0>, x<1>, x<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(z, x<1>, x<2>), is(z)),
                    prod(list(z), list(x<1>, x<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(x<0>, z, x<2>), is(z)),
                    prod(list(), list(x<0>, z, x<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(z, z, x<2>), is(z)),
                    prod(list(z, z), list(x<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    span(list(z, z, z), is(z)),
                    prod(list(z, z, z), list())
                ));
            }

            // take_while
            {
                auto z = x<999>;
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(), isnt(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(x<1>), isnt(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(z), isnt(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(x<1>, x<2>), isnt(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(x<1>, z), isnt(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(z, x<2>), isnt(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(x<1>, x<2>, x<3>), isnt(z)),
                    list(x<1>, x<2>, x<3>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(x<1>, x<2>, z), isnt(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(x<1>, z, x<3>), isnt(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_while(list(z, x<2>, x<3>), isnt(z)),
                    list()
                ));
            }

            // take_until
            {
                auto z = x<999>;

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(), is(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(x<1>), is(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(z), is(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(x<1>, x<2>), is(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(x<1>, z), is(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(z, x<2>), is(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(x<1>, x<2>, x<3>), is(z)),
                    list(x<1>, x<2>, x<3>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(x<1>, x<2>, z), is(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(x<1>, z, x<3>), is(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    take_until(list(z, x<2>, x<3>), is(z)),
                    list()
                ));
            }

            // permutations
            {
                auto permute = [=](auto xs) {
                    return [=](auto ...expected_) {
                        auto actual = permutations(xs);
                        auto expected = list(expected_...);

                        BOOST_HANA_CONSTANT_ASSERT(and_(
                            equal(length(expected), length(actual)),
                            all(actual, [=](auto x) { return elem(expected, x); })
                        ));
                    };
                };

                BOOST_HANA_CONSTANT_ASSERT(equal(permutations(list()), list(list())));

                permute(list(x<0>))(list(x<0>));
                permute(list(x<0>, x<1>))(
                    list(x<0>, x<1>),
                    list(x<1>, x<0>)
                );
                permute(list(x<0>, x<1>, x<2>))(
                    list(x<0>, x<1>, x<2>),
                    list(x<0>, x<2>, x<1>),
                    list(x<1>, x<0>, x<2>),
                    list(x<1>, x<2>, x<0>),
                    list(x<2>, x<0>, x<1>),
                    list(x<2>, x<1>, x<0>)
                );
            }

            // filter
            {
                auto z = x<999>;

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(), isnt(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(z), isnt(z)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(x<1>), isnt(z)),
                    list(x<1>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(x<1>, x<2>), isnt(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(z, x<2>), isnt(z)),
                    list(x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(x<1>, z), isnt(z)),
                    list(x<1>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(z, x<2>, x<3>), isnt(z)),
                    list(x<2>, x<3>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(x<1>, z, x<3>), isnt(z)),
                    list(x<1>, x<3>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(x<1>, x<2>, z), isnt(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(x<1>, z, z), isnt(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(z, x<2>, z), isnt(z)),
                    list(x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    filter(list(z, z, x<3>), isnt(z)),
                    list(x<3>)
                ));
            }

            // group
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<0>)),
                    list(list(x<0>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<0>, x<0>)),
                    list(list(x<0>, x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<0>, x<1>)),
                    list(list(x<0>), list(x<1>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<0>, x<0>, x<0>)),
                    list(list(x<0>, x<0>, x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<0>, x<0>, x<1>)),
                    list(list(x<0>, x<0>), list(x<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<0>, x<1>, x<0>)),
                    list(list(x<0>), list(x<1>), list(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<1>, x<0>, x<0>)),
                    list(list(x<1>), list(x<0>, x<0>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<0>, x<0>, x<1>, x<1>)),
                    list(list(x<0>, x<0>), list(x<1>, x<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group(list(x<0>, x<0>, x<1>, x<1>, x<2>, x<2>, x<2>)),
                    list(list(x<0>, x<0>), list(x<1>, x<1>), list(x<2>, x<2>, x<2>))
                ));
            }

            // partition
            {
                auto pred = [=](auto z) {
                    return elem(list(x<-1>, x<-2>, x<-3>, x<-4>, x<-5>), z);
                };

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    partition(list(), pred),
                    prod(list(), list())
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    partition(list(x<0>), pred),
                    prod(list(), list(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    partition(list(x<0>, x<1>), pred),
                    prod(list(), list(x<0>, x<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    partition(list(x<-1>), pred),
                    prod(list(x<-1>), list())
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    partition(list(x<-1>, x<0>, x<2>), pred),
                    prod(list(x<-1>), list(x<0>, x<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    partition(list(x<0>, x<-3>, x<2>, x<-5>, x<6>), pred),
                    prod(list(x<-3>, x<-5>), list(x<0>, x<2>, x<6>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    partition(list(x<-1>, x<2>, x<-3>, x<0>, x<-3>, x<4>), pred),
                    prod(list(x<-1>, x<-3>, x<-3>), list(x<2>, x<0>, x<4>))
                ));
            }

            // scans
            {
                auto f = injection([]{});
                auto s = x<999>;

                // scanl
                {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl(list(), s, f),
                        list(s)
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl(list(x<0>), s, f),
                        list(s, f(s, x<0>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl(list(x<0>, x<1>), s, f),
                        list(s, f(s, x<0>), f(f(s, x<0>), x<1>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl(list(x<0>, x<1>, x<2>), s, f),
                        list(s, f(s, x<0>), f(f(s, x<0>), x<1>), f(f(f(s, x<0>), x<1>), x<2>))
                    ));
                }

                // scanr
                {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanr(list(), s, f),
                        list(s)
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanr(list(x<0>), s, f),
                        list(f(x<0>, s), s)
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanr(list(x<0>, x<1>), s, f),
                        list(f(x<0>, f(x<1>, s)), f(x<1>, s), s)
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanr(list(x<0>, x<1>, x<2>), s, f),
                        list(f(x<0>, f(x<1>, f(x<2>, s))), f(x<1>, f(x<2>, s)), f(x<2>, s), s)
                    ));
                }

                // scanl1
                {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl1(list(), f),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl1(list(x<0>), f),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl1(list(x<0>, x<1>), f),
                        list(x<0>, f(x<0>, x<1>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl1(list(x<0>, x<1>, x<2>), f),
                        list(x<0>, f(x<0>, x<1>), f(f(x<0>, x<1>), x<2>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanl1(list(x<0>, x<1>, x<2>, x<3>), f),
                        list(x<0>, f(x<0>, x<1>), f(f(x<0>, x<1>), x<2>), f(f(f(x<0>, x<1>), x<2>), x<3>))
                    ));
                }

                // scanr1
                {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanr1(list(), f),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanr1(list(x<0>), f),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanr1(list(x<0>, x<1>), f),
                        list(f(x<0>, x<1>), x<1>)
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        scanr1(list(x<0>, x<1>, x<2>), f),
                        list(f(x<0>, f(x<1>, x<2>)), f(x<1>, x<2>), x<2>)
                    ));
                }
            }

            // unfolds
            {
                // unfoldl
                {
                    auto f = injection([]{});
                    auto stop_at = [=](auto stop) {
                        return [=](auto x) {
                            return if_(equal(stop, x),
                                nothing,
                                just(prod(x + int_<1>, f(x)))
                            );
                        };
                    };

                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldl<L>(stop_at(int_<0>), int_<0>),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldl<L>(stop_at(int_<1>), int_<0>),
                        list(f(int_<0>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldl<L>(stop_at(int_<2>), int_<0>),
                        list(f(int_<1>), f(int_<0>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldl<L>(stop_at(int_<3>), int_<0>),
                        list(f(int_<2>), f(int_<1>), f(int_<0>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldl<L>(stop_at(int_<4>), int_<0>),
                        list(f(int_<3>), f(int_<2>), f(int_<1>), f(int_<0>))
                    ));
                }

                // unfoldr
                {
                    auto f = injection([]{});
                    auto stop_at = [=](auto stop) {
                        return [=](auto x) {
                            return if_(equal(stop, x),
                                nothing,
                                just(prod(f(x), x + int_<1>))
                            );
                        };
                    };

                    BOOST_HANA_CONSTANT_ASSERT( equal(
                        unfoldr<L>(stop_at(int_<0>), int_<0>),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldr<L>(stop_at(int_<1>), int_<0>),
                        list(f(int_<0>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldr<L>(stop_at(int_<2>), int_<0>),
                        list(f(int_<0>), f(int_<1>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldr<L>(stop_at(int_<3>), int_<0>),
                        list(f(int_<0>), f(int_<1>), f(int_<2>))
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        unfoldr<L>(stop_at(int_<4>), int_<0>),
                        list(f(int_<0>), f(int_<1>), f(int_<2>), f(int_<3>))
                    ));
                }

                // Make sure unfold{r,l} can be reversed under certain conditions.
                {
                    auto z = x<999>;
                    auto f = prod;
                    auto g = [=](auto k) { return if_(equal(k, z), nothing, just(k)); };

                    // Make sure the special conditions are met
                    BOOST_HANA_CONSTANT_ASSERT(equal(g(z), nothing));
                    BOOST_HANA_CONSTANT_ASSERT(equal(g(f(x<0>, z)), just(prod(x<0>, z))));
                    BOOST_HANA_CONSTANT_ASSERT(equal(g(f(z, x<0>)), just(prod(z, x<0>))));

                    // Make sure the reversing works
                    for_each(list(list(), list(x<0>), list(x<0>, x<1>), list(x<0>, x<1>, x<2>)),
                        [=](auto xs) {
                            BOOST_HANA_CONSTANT_ASSERT(equal(
                                unfoldl<L>(g, foldl(xs, z, f)),
                                xs
                            ));
                            BOOST_HANA_CONSTANT_ASSERT(equal(
                                unfoldr<L>(g, foldr(xs, z, f)),
                                xs
                            ));
                        });
                }
            }

            // sort_by
            {

                auto pred = [](auto x, auto y) {
                    return less(x.unwrap, y.unwrap);
                };
                auto a = [](auto z) { return test::tag(x<999>, z); };
                auto b = [](auto z) { return test::tag(x<888>, z); };

                auto check = [=](auto ...sorted) {
                    auto perms = fmap(
                        permutations(list(a(sorted)...)),
                        partial(sort_by, pred)
                    );
                    BOOST_HANA_CONSTANT_ASSERT(all(perms, [=](auto xs) {
                        return equal(xs, list(a(sorted)...));
                    }));
                };

                check();
                check(ord<1>);
                check(ord<1>, ord<2>);
                check(ord<1>, ord<2>, ord<3>);

                // check stability
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort_by(pred, list(a(ord<1>), b(ord<1>))),
                    list(a(ord<1>), b(ord<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort_by(pred, list(b(ord<1>), a(ord<1>))),
                    list(b(ord<1>), a(ord<1>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort_by(pred, list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))),
                    list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort_by(pred, list(a(ord<1>), a(ord<2>), b(ord<1>), b(ord<2>))),
                    list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort_by(pred, list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))),
                    list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort_by(pred, list(a(ord<2>), b(ord<1>), b(ord<2>), a(ord<1>))),
                    list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sort_by(pred, list(a(ord<1>), a(ord<3>), b(ord<1>), a(ord<2>), b(ord<3>))),
                    list(a(ord<1>), b(ord<1>), a(ord<2>), a(ord<3>), b(ord<3>))
                ));
            }

            // group_by
            {
                auto a = [](auto z) { return test::tag(x<999>, z); };
                auto b = [](auto z) { return test::tag(x<888>, z); };

                BOOST_HANA_CONSTEXPR_LAMBDA auto pred = [](auto x, auto y) {
                    return equal(x.unwrap, y.unwrap);
                };

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group_by(pred, list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group_by(pred, list(a(x<0>))),
                    list(list(a(x<0>)))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group_by(pred, list(a(x<0>), b(x<0>))),
                    list(list(a(x<0>), b(x<0>)))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group_by(pred, list(a(x<0>), b(x<0>), a(x<1>))),
                    list(list(a(x<0>), b(x<0>)), list(a(x<1>)))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group_by(pred, list(a(x<0>), b(x<0>), a(x<1>), b(x<1>))),
                    list(list(a(x<0>), b(x<0>)), list(a(x<1>), b(x<1>)))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    group_by(pred, list(a(x<0>), b(x<0>), a(x<1>), b(x<1>), b(x<0>))),
                    list(list(a(x<0>), b(x<0>)), list(a(x<1>), b(x<1>)), list(b(x<0>)))
                ));
            }

            // zip
            {
                auto t = list; // tests are unreadable otherwise. mnemonic: tuple
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip(list(x<0>)),
                    list(t(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip(list(x<0>, x<1>)),
                    list(t(x<0>), t(x<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip(list(x<0>, x<1>, x<2>)),
                    list(t(x<0>), t(x<1>), t(x<2>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip(list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip(list(x<0>), list(x<1>)),
                    list(t(x<0>, x<1>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip(list(x<0>), list(x<1>), list(x<2>)),
                    list(t(x<0>, x<1>, x<2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip(list(x<0>, x<3>), list(x<1>, x<4>), list(x<2>, x<5>, x<8>)),
                    list(t(x<0>, x<1>, x<2>), t(x<3>, x<4>, x<5>))
                ));
            }

            // zip_with
            {
                auto f = injection([]{});

                // zip 1
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<0>)),
                    list(f(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<0>, x<1>)),
                    list(f(x<0>), f(x<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<0>, x<1>, x<2>)),
                    list(f(x<0>), f(x<1>), f(x<2>))
                ));

                // zip 2
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(undefined), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(), list(undefined)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<1>), list(x<-1>)),
                    list(f(x<1>, x<-1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<1>, x<2>), list(x<-1>)),
                    list(f(x<1>, x<-1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<1>), list(x<-1>, x<-2>)),
                    list(f(x<1>, x<-1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<1>, x<2>), list(x<-1>, x<-2>)),
                    list(f(x<1>, x<-1>), f(x<2>, x<-2>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<1>, x<2>, x<3>, x<4>), list(x<-1>, x<-2>, x<-3>)),
                    list(f(x<1>, x<-1>), f(x<2>, x<-2>), f(x<3>, x<-3>))
                ));

                // zip 3
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(),          list(),          list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(undefined), list(),          list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(),          list(undefined), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(),          list(),          list(undefined)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(),          list(undefined), list(undefined)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(undefined), list(),          list(undefined)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(undefined, list(undefined), list(undefined), list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f, list(x<0>), list(x<1>), list(x<2>)),
                    list(f(x<0>, x<1>, x<2>))
                ));

                // zip 4
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f,
                        list(x<11>, x<12>, x<13>, x<14>),
                        list(x<21>, x<22>, x<23>),
                        list(x<31>, x<32>, x<33>, x<34>),
                        list(x<41>, x<42>, x<43>, x<44>, x<45>)
                    ),
                    list(
                        f(x<11>, x<21>, x<31>, x<41>),
                        f(x<12>, x<22>, x<32>, x<42>),
                        f(x<13>, x<23>, x<33>, x<43>)
                    )
                ));

                // zip 5
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    zip_with(f,
                        list(x<11>, x<12>, x<13>, x<14>),
                        list(x<21>, x<22>, x<23>, x<24>, x<25>),
                        list(x<31>, x<32>, x<33>, x<34>),
                        list(x<41>, x<42>, x<43>, x<44>, x<45>, x<46>),
                        list(x<51>, x<52>, x<53>, x<54>, x<55>)
                    ),
                    list(
                        f(x<11>, x<21>, x<31>, x<41>, x<51>),
                        f(x<12>, x<22>, x<32>, x<42>, x<52>),
                        f(x<13>, x<23>, x<33>, x<43>, x<53>),
                        f(x<14>, x<24>, x<34>, x<44>, x<54>)
                    )
                ));
            }
        }

        // Functor instance
        {
            laws<Functor, L>();

            auto f = injection([]{});

            // fmap
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    fmap(list(), f),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    fmap(list(x<1>), f),
                    list(f(x<1>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    fmap(list(x<1>, x<2>), f),
                    list(f(x<1>), f(x<2>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    fmap(list(x<1>, x<2>, x<3>), f),
                    list(f(x<1>), f(x<2>), f(x<3>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    fmap(list(x<1>, x<2>, x<3>, x<4>), f),
                    list(f(x<1>), f(x<2>), f(x<3>), f(x<4>))
                ));
            }
        }

        // Applicative instance
        {
            laws<Applicative, L>();

            using test::x;
            auto f = test::injection([]{});
            auto g = test::injection([]{});

            // ap
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(), list(x<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(), list(x<0>, x<1>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(), list(x<0>, x<1>, x<2>)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(f), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(f), list(x<0>)),
                    list(f(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(f), list(x<0>, x<1>)),
                    list(f(x<0>), f(x<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(f), list(x<0>, x<1>, x<2>)),
                    list(f(x<0>), f(x<1>), f(x<2>))
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(f, g), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(f, g), list(x<0>)),
                    list(f(x<0>), g(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(f, g), list(x<0>, x<1>)),
                    list(f(x<0>), f(x<1>), g(x<0>), g(x<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    ap(list(f, g), list(x<0>, x<1>, x<2>)),
                    list(f(x<0>), f(x<1>), f(x<2>), g(x<0>), g(x<1>), g(x<2>))
                ));
            }

            // lift
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(lift<L>(x<0>), list(x<0>)));
                BOOST_HANA_CONSTANT_ASSERT(equal(lift<L>(x<1>), list(x<1>)));
            }
        }

        // Monad instance
        {
            laws<Monad, L>();

            using test::x;

            // flatten
            {
                BOOST_HANA_CONSTANT_ASSERT(
                    equal(flatten(list(list(), list())),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(
                    equal(flatten(list(list(x<0>), list())),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(
                    equal(flatten(list(list(), list(x<0>))),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(
                    equal(flatten(list(list(x<0>), list(x<1>))),
                    list(x<0>, x<1>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    flatten(list(list(x<0>, x<1>), list(), list(x<2>, x<3>), list(x<4>))),
                    list(x<0>, x<1>, x<2>, x<3>, x<4>)
                ));
            }

            // ...
        }

        // Traversable instance
        {
            laws<Traversable, L>();

            auto f = test::injection([]{});
            auto applicative = test::identity;
            using A = test::Identity;
            using test::x;

            // traverse
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    traverse<A>(list(), compose(applicative, f)),
                    applicative(list())
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    traverse<A>(list(x<0>), compose(applicative, f)),
                    applicative(list(f(x<0>)))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    traverse<A>(list(x<0>, x<1>), compose(applicative, f)),
                    applicative(list(f(x<0>), f(x<1>)))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    traverse<A>(list(x<0>, x<1>, x<2>), compose(applicative, f)),
                    applicative(list(f(x<0>), f(x<1>), f(x<2>)))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    traverse<A>(list(x<0>, x<1>, x<2>, x<3>), compose(applicative, f)),
                    applicative(list(f(x<0>), f(x<1>), f(x<2>), f(x<3>)))
                ));
            }

            // sequence
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sequence<A>(list()),
                    applicative(list())
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sequence<A>(list(applicative(x<0>))),
                    applicative(list(x<0>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sequence<A>(list(applicative(x<0>), applicative(x<1>))),
                    applicative(list(x<0>, x<1>))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    sequence<A>(list(applicative(x<0>), applicative(x<1>), applicative(x<2>))),
                    applicative(list(x<0>, x<1>, x<2>))
                ));
            }
        }

        // Comparable instance
        {
            laws<Comparable, L>();
            using test::x;

            // equal
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    list(),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(not_(equal(
                    list(x<0>),
                    list()
                )));
                BOOST_HANA_CONSTANT_ASSERT(not_(equal(
                    list(),
                    list(x<0>)
                )));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    list(x<0>),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(not_(equal(
                    list(x<0>, x<1>),
                    list(x<0>)
                )));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    list(x<0>, x<1>),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(not_(equal(
                    list(x<0>, x<1>, x<2>, x<3>),
                    list(x<0>, x<1>, x<2>, x<4>)
                )));
            }
        }

        // Foldable
        //
        // Foldable is actually provided by Iterable, but it is very easy for
        // us to check that instance.
        {
            auto f = test::injection([]{});
            using test::x;
            constexpr struct { } undefined{};

            // foldl
            {
                auto s = x<999>;
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldl(list(x<1>), s, f),
                    f(s, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldl(list(x<1>, x<2>), s, f),
                    f(f(s, x<1>), x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldl(list(x<1>, x<2>, x<3>), s, f),
                    f(f(f(s, x<1>), x<2>), x<3>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldl(list(x<1>, x<2>, x<3>, x<4>), s, f),
                    f(f(f(f(s, x<1>), x<2>), x<3>), x<4>)
                ));
            }

            // foldl1
            {
                auto s = x<999>;
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldl1(list(s), f),
                    s
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldl1(list(s, x<2>), f),
                    f(s, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldl1(list(s, x<2>, x<3>), f),
                    f(f(s, x<2>), x<3>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldl1(list(s, x<2>, x<3>, x<4>), f),
                    f(f(f(s, x<2>), x<3>), x<4>)
                ));
            }

            // foldr
            {
                auto s = x<999>;
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldr(list(x<0>), s, f),
                    f(x<0>, s)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldr(list(x<0>, x<1>), s, f),
                    f(x<0>, f(x<1>, s))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldr(list(x<0>, x<1>, x<2>), s, f),
                    f(x<0>, f(x<1>, f(x<2>, s)))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldr(list(x<0>, x<1>, x<2>, x<3>), s, f),
                    f(x<0>, f(x<1>, f(x<2>, f(x<3>, s))))
                ));
            }

            // foldr1
            {
                auto s = x<999>;
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldr1(list(s), f),
                    s
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldr1(list(x<1>, s), f),
                    f(x<1>, s)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldr1(list(x<1>, x<2>, s), f),
                    f(x<1>, f(x<2>, s))
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    foldr1(list(x<1>, x<2>, x<3>, s), f),
                    f(x<1>, f(x<2>, f(x<3>, s)))
                ));
            }

            // for_each
            {
                auto check = [=](auto ...xs) {
                    std::vector<int> seen{};
                    for_each(list(xs...), [&](int x) {
                        seen.push_back(x);
                    });
                    BOOST_HANA_RUNTIME_ASSERT(seen == std::vector<int>{xs...});
                };
                check();
                check(0);
                check(0, 1);
                check(0, 1, 2);
                check(0, 1, 2, 3);
                check(0, 1, 2, 3, 4);
            }

            // length
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    length(list()), size_t<0>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    length(list(undefined)), size_t<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    length(list(undefined, undefined)), size_t<2>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    length(list(undefined, undefined, undefined)), size_t<3>
                ));
            }

            // maximum
            {
                {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<0>)),
                        ord<0>
                    ));

                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<0>, ord<1>)),
                        ord<1>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<1>, ord<0>)),
                        ord<1>
                    ));

                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<0>, ord<1>, ord<2>)),
                        ord<2>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<1>, ord<0>, ord<2>)),
                        ord<2>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<1>, ord<2>, ord<0>)),
                        ord<2>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<2>, ord<1>, ord<0>)),
                        ord<2>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<2>, ord<0>, ord<1>)),
                        ord<2>
                    ));

                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<0>, ord<1>, ord<2>, ord<3>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<1>, ord<0>, ord<2>, ord<3>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<1>, ord<2>, ord<0>, ord<3>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<1>, ord<2>, ord<3>, ord<0>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<2>, ord<1>, ord<3>, ord<0>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<2>, ord<3>, ord<1>, ord<0>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<2>, ord<3>, ord<0>, ord<1>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<3>, ord<2>, ord<0>, ord<1>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<3>, ord<0>, ord<2>, ord<1>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<3>, ord<0>, ord<1>, ord<2>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<0>, ord<2>, ord<3>, ord<1>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        maximum(list(ord<0>, ord<3>, ord<1>, ord<2>)),
                        ord<3>
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{0})),
                        int{0}
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{0}, char{1})),
                        char{1}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{1}, char{0})),
                        int{1}
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{0}, char{1}, long{2})),
                        long{2}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{1}, char{0}, long{2})),
                        long{2}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{1}, char{2}, long{0})),
                        char{2}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{2}, char{1}, long{0})),
                        int{2}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{2}, char{0}, long{1})),
                        int{2}
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{0}, char{1}, long{2}, double{3})),
                        double{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{1}, char{0}, long{2}, double{3})),
                        double{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{1}, char{2}, long{0}, double{3})),
                        double{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{1}, char{2}, long{3}, double{0})),
                        long{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{2}, char{1}, long{3}, double{0})),
                        long{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{2}, char{3}, long{1}, double{0})),
                        char{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{2}, char{3}, long{0}, double{1})),
                        char{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{3}, char{2}, long{0}, double{1})),
                        int{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{3}, char{0}, long{2}, double{1})),
                        int{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{3}, char{0}, long{1}, double{2})),
                        int{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{0}, char{2}, long{3}, double{1})),
                        long{3}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{0}, char{3}, long{1}, double{2})),
                        char{3}
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{0}, char{1}, long_<2>)),
                        long{2}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        maximum(list(int{0}, long_<1>, char{2})),
                        char{2}
                    ));
                }
            }

            // minimum
            {
                {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<0>)),
                        ord<0>
                    ));

                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<0>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<1>, ord<0>)),
                        ord<0>
                    ));

                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<0>, ord<1>, ord<2>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<1>, ord<0>, ord<2>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<1>, ord<2>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<2>, ord<1>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<2>, ord<0>, ord<1>)),
                        ord<0>
                    ));

                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<0>, ord<1>, ord<2>, ord<3>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<1>, ord<0>, ord<2>, ord<3>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<1>, ord<2>, ord<0>, ord<3>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<1>, ord<2>, ord<3>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<2>, ord<1>, ord<3>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<2>, ord<3>, ord<1>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<2>, ord<3>, ord<0>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<3>, ord<2>, ord<0>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<3>, ord<0>, ord<2>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<3>, ord<0>, ord<1>, ord<2>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<0>, ord<2>, ord<3>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        minimum(list(ord<0>, ord<3>, ord<1>, ord<2>)),
                        ord<0>
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{0})),
                        int{0}
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{0}, char{1})),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{1}, char{0})),
                        char{0}
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{0}, char{1}, long{2})),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{1}, char{0}, long{2})),
                        char{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{1}, char{2}, long{0})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{2}, char{1}, long{0})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{2}, char{0}, long{1})),
                        char{0}
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{0}, char{1}, long{2}, double{3})),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{1}, char{0}, long{2}, double{3})),
                        char{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{1}, char{2}, long{0}, double{3})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{1}, char{2}, long{3}, double{0})),
                        double{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{2}, char{1}, long{3}, double{0})),
                        double{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{2}, char{3}, long{1}, double{0})),
                        double{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{2}, char{3}, long{0}, double{1})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{3}, char{2}, long{0}, double{1})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{3}, char{0}, long{2}, double{1})),
                        char{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{3}, char{0}, long{1}, double{2})),
                        char{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{0}, char{2}, long{3}, double{1})),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{0}, char{3}, long{1}, double{2})),
                        int{0}
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(int{0}, char{1}, long_<2>)),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        minimum(list(ord<0>, char{1}, long{2})),
                        ord<0>
                    ));
                }
            }

            // count
            {
                constexpr auto f = id;

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(), f), size_t<0>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<1>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<0>), f), size_t<0>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<1>, char_<1>), f), size_t<2>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<1>, char_<0>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<0>, char_<1>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<0>, char_<0>), f), size_t<0>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<1>, char_<1>, long_<1>), f), size_t<3>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<1>, char_<1>, long_<0>), f), size_t<2>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<1>, char_<0>, long_<1>), f), size_t<2>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<1>, char_<0>, long_<0>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<0>, char_<1>, long_<1>), f), size_t<2>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<0>, char_<1>, long_<0>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<0>, char_<0>, long_<1>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    count(list(int_<0>, char_<0>, long_<0>), f), size_t<0>
                ));


                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{1}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{0}), f), 0u
                ));

                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{1}, char{1}), f), 2u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{1}, char{0}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{0}, char{1}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{0}, char{0}), f), 0u
                ));

                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{1}, char{1}, double{1}), f), 3u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{1}, char{1}, double{0}), f), 2u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{1}, char{0}, double{1}), f), 2u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{1}, char{0}, double{0}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{0}, char{1}, double{1}), f), 2u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{0}, char{1}, double{0}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{0}, char{0}, double{1}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_ASSERT(equal(
                    count(list(int{0}, char{0}, double{0}), f), 0u
                ));
            }

            // product
            {
                {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        product(list()),
                        int_<1>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        product(list(int_<2>)),
                        int_<2>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        product(list(int_<2>, int_<3>)),
                        int_<2 * 3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        product(list(int_<2>, int_<3>, int_<4>)),
                        int_<2 * 3 * 4>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        product(list(int_<2>, int_<3>, int_<4>, int_<5>)),
                        int_<2 * 3 * 4 * 5>
                    ));
                }
                {
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        product(list(2)),
                        2
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        product(list(2, 3)),
                        2 * 3
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        product(list(2, 3, 4)),
                        2 * 3 * 4
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        product(list(2, 3, 4, 5)),
                        2 * 3 * 4 * 5
                    ));
                }
            }

            // sum
            {
                {
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        sum(list()),
                        int_<0>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        sum(list(int_<1>)),
                        int_<1>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        sum(list(int_<1>, int_<2>)),
                        int_<1 + 2>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        sum(list(int_<1>, int_<2>, int_<3>)),
                        int_<1 + 2 + 3>
                    ));
                    BOOST_HANA_CONSTANT_ASSERT(equal(
                        sum(list(int_<1>, int_<2>, int_<3>, int_<4>)),
                        int_<1 + 2 + 3 + 4>
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        sum(list(1)), 1
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        sum(list(1, 2)), 1 + 2
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        sum(list(1, 2, 3)), 1 + 2 + 3
                    ));
                    BOOST_HANA_CONSTEXPR_ASSERT(equal(
                        sum(list(1, 2, 3, 4)), 1 + 2 + 3 + 4
                    ));
                }
            }

            // unpack
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unpack(list(), f),
                    f()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unpack(list(x<0>), f),
                    f(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unpack(list(x<0>, x<1>), f),
                    f(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unpack(list(x<0>, x<1>, x<2>), f),
                    f(x<0>, x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    unpack(list(x<0>, x<1>, x<2>, x<3>), f),
                    f(x<0>, x<1>, x<2>, x<3>)
                ));
            }
        }

        // Searchable
        //
        // Searchable is actually provided by Iterable, but it is very easy
        // for us to check that instance.
        {
            auto is = [](auto x) {
                return [=](auto y) { return equal(x, y); };
            };

            auto c = test::numeric; // a minimal comparable
            auto logical = test::numeric;

            // any
            {
                // compile-time
                {
                    BOOST_HANA_CONSTANT_ASSERT(not_(any(list(), is(x<9>))));

                    BOOST_HANA_CONSTANT_ASSERT(not_(any(list(x<0>), is(x<9>))));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>), is(x<0>)));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>, invalid<1>), is(x<0>)));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>, invalid<1>, invalid<2>), is(x<0>)));

                    BOOST_HANA_CONSTANT_ASSERT(not_(any(list(x<0>, x<1>), is(x<9>))));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>, x<1>), is(x<1>)));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>, x<1>, invalid<2>), is(x<1>)));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>, x<1>, invalid<2>, invalid<3>), is(x<1>)));

                    BOOST_HANA_CONSTANT_ASSERT(not_(any(list(x<0>, x<1>, x<2>), is(x<9>))));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>, x<1>, x<2>), is(x<2>)));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>, x<1>, x<2>, nothing), is(x<2>)));
                    BOOST_HANA_CONSTANT_ASSERT(any(list(x<0>, x<1>, x<2>, nothing, nothing), is(x<2>)));
                }

                // runtime
                {
                    BOOST_HANA_CONSTANT_ASSERT(not_(
                        any(list(), undefined)
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        any(list(c(0)), is(c(0)))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        not_(any(list(c(0)), is(c(1))))
                    );
                }
            }

            // any_of
            {
                // runtime
                {
                    BOOST_HANA_CONSTANT_ASSERT(not_(
                        any_of(list())
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        any_of(list(logical(true)))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        not_(any_of(list(logical(false))))
                    );
                }
            }

            // all
            {
                // runtime
                {
                    BOOST_HANA_CONSTANT_ASSERT(all(list(), undefined));

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        all(list(c(0)), is(c(0)))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        not_(all(list(c(0)), is(c(1))))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(not_(
                        all(list(c(0), c(1)), is(c(0)))
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        all(list(c(0), c(0)), is(c(0)))
                    );
                }
            }

            // all_of
            {
                BOOST_HANA_CONSTANT_ASSERT(
                    all_of(list())
                );

                BOOST_HANA_CONSTEXPR_ASSERT(
                    all_of(list(logical(true)))
                );

                BOOST_HANA_CONSTEXPR_ASSERT(
                    not_(all_of(list(logical(false))))
                );

                BOOST_HANA_CONSTEXPR_ASSERT(
                    all_of(list(logical(true), logical(true)))
                );

                BOOST_HANA_CONSTEXPR_ASSERT(not_(
                    all_of(list(logical(true), logical(false)))
                ));
            }

            // none
            {
                // runtime
                {
                    BOOST_HANA_CONSTANT_ASSERT(none(list(), undefined));

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        none(list(c(0)), is(c(1)))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        not_(none(list(c(0)), is(c(0))))
                    );
                }
            }

            // none_of
            {
                // runtime
                {
                    BOOST_HANA_CONSTANT_ASSERT(none_of(list()));

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        none_of(list(logical(false)))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        not_(none_of(list(logical(true))))
                    );
                }
            }

            // find
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(), is(x<9>)),
                    nothing
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>), is(x<9>)),
                    nothing
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>), is(x<0>)),
                    just(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, invalid<1>), is(x<0>)),
                    just(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, invalid<1>, invalid<2>), is(x<0>)),
                    just(x<0>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, x<1>), is(x<9>)),
                    nothing
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, x<1>), is(x<1>)),
                    just(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, x<1>, invalid<2>), is(x<1>)),
                    just(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, x<1>, invalid<2>, invalid<3>), is(x<1>)),
                    just(x<1>)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, x<1>, x<2>), is(x<9>)),
                    nothing
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, x<1>, x<2>), is(x<2>)),
                    just(x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, x<1>, x<2>, nothing), is(x<2>)),
                    just(x<2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    find(list(x<0>, x<1>, x<2>, nothing, nothing), is(x<2>)),
                    just(x<2>)
                ));
            }

            // elem
            {
                // runtime
                {
                    BOOST_HANA_CONSTEXPR_ASSERT(
                        elem(list(c(0)), c(0))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(
                        not_(elem(list(c(0)), c(1)))
                    );
                }
            }

            // in
            {
                // runtime
                {
                    BOOST_HANA_CONSTEXPR_ASSERT(
                        c(0) ^in^ list(c(0))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(not_(
                        c(1) ^in^ list(c(0))
                    ));
                }
            }

            // subset
            {
                // runtime
                {
                    BOOST_HANA_CONSTANT_ASSERT(subset(
                        list(), list(c(1)))
                    );

                    BOOST_HANA_CONSTEXPR_ASSERT(subset(
                        list(c(0)), list(c(0))
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(subset(
                        list(c(0)), list(c(0), c(1))
                    ));

                    BOOST_HANA_CONSTEXPR_ASSERT(not_(subset(
                        list(c(0)), list(c(1)))
                    ));
                }
            }
        }

        // Iterable
        //
        // While List does not _provide_ an instance for Iterable, it requires
        // any instance to also instantiate Iterable. Furthermore, since all List
        // instances are isomorphic, it is possible to check the Iterable instance
        // of any List.
        {
            laws<Iterable, L>();

            using test::x;
            using list_detail::invalid;
            using list_detail::non_pod;

            // head
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    head(list(x<0>)),
                    x<0>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    head(list(x<0>, invalid<>)),
                    x<0>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    head(list(x<0>, invalid<1>, invalid<2>)),
                    x<0>
                ));
            }

            // is_empty
            {
                BOOST_HANA_CONSTANT_ASSERT(is_empty(list()));
                BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(list(invalid<>))));
                BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(list(invalid<0>, invalid<1>))));
            }

            // tail
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    tail(list(invalid<>, x<0>)), list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    tail(list(invalid<>, x<0>, x<1>)), list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    tail(list(invalid<>, x<0>, x<1>, x<2>)), list(x<0>, x<1>, x<2>)
                ));
            }

            // at
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    at(size_t<0>, list(x<0>)), x<0>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    at(size_t<0>, list(x<0>, invalid<>)), x<0>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    at(size_t<1>, list(invalid<>, x<1>)), x<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    at(size_t<1>, list(invalid<0>, x<1>, invalid<2>)), x<1>
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    at(size_t<2>, list(invalid<0>, invalid<1>, x<2>)), x<2>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    at(size_t<2>, list(invalid<0>, invalid<1>, x<2>, invalid<3>)), x<2>
                ));

                // make sure we can use non-pods on both sides
                at(size_t<1>, list(non_pod<0>, x<1>, non_pod<2>));
            }

            // last
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    last(list(x<0>)),
                    x<0>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    last(list(invalid<0>, x<1>)),
                    x<1>
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    last(list(invalid<0>, invalid<1>, x<2>)),
                    x<2>
                ));
            }

            // drop
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<0>, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<1>, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<2>, list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<0>, list(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<1>, list(x<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<2>, list(x<0>)),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<0>, list(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<1>, list(x<0>, x<1>)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop(size_t<2>, list(x<0>, x<1>)),
                    list()
                ));
            }

            // drop_until
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_until(list(), id),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_until(list(true_), id),
                    list(true_)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_until(list(false_), id),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_until(list(true_, true_), id),
                    list(true_, true_)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_until(list(true_, false_), id),
                    list(true_, false_)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_until(list(false_, true_), id),
                    list(true_)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_until(list(false_, false_), id),
                    list()
                ));
            }

            // drop_while
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_while(list(), id),
                    list()
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_while(list(true_), id),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_while(list(false_), id),
                    list(false_)
                ));

                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_while(list(true_, true_), id),
                    list()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_while(list(true_, false_), id),
                    list(false_)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_while(list(false_, true_), id),
                    list(false_, true_)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    drop_while(list(false_, false_), id),
                    list(false_, false_)
                ));
            }
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_LIST_HPP
