/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_LIST_HPP
#define BOOST_HANA_TEST_TEST_AUTO_LIST_HPP

#include <boost/hana/sequence.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
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

#include <type_traits>
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
        BOOST_HANA_CONSTANT_CHECK(models<List, L>);

        auto list = make<L>;
        auto foldable = seq;
        using list_detail::ord;
        using list_detail::invalid;
        constexpr struct { } undefined{};

        // Check for basic data type consistency
        {
            for_each(objects<L>, [](auto xs) {
                static_assert(std::is_same<
                    datatype_t<decltype(xs)>, L
                >::value, "");
            });

            static_assert(std::is_same<
                datatype_t<decltype(list())>, L
            >::value, "");

            static_assert(std::is_same<
                datatype_t<decltype(list(1))>, L
            >::value, "");

            static_assert(std::is_same<
                datatype_t<decltype(list(1, '2'))>, L
            >::value, "");

            static_assert(std::is_same<
                datatype_t<decltype(list(1, '2', 3.3))>, L
            >::value, "");

            struct Random;
            static_assert(!std::is_same<
                datatype_t<Random>, L
            >::value, "");
        }

        // Instance-wide laws
        {
            for_each(objects<L>, [](auto xs) {
                for_each(objects<L>, [=](auto ys) {
                    BOOST_HANA_CHECK(
                        equal(xs, ys) ^iff^ equal(to<Tuple>(xs), to<Tuple>(ys))
                    );
                });
            });
        }

        // Conversions
        {
            // Foldable -> List
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<L>(foldable()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<L>(foldable(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<L>(foldable(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<L>(foldable(x<0>, x<1>, x<2>)),
                    list(x<0>, x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
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
                BOOST_HANA_CONSTANT_CHECK(equal(
                    make<L>(),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    make<L>(x<0>),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    make<L>(x<0>, x<1>),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    make<L>(x<0>, x<1>, x<2>),
                    list(x<0>, x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    make<L>(x<0>, x<1>, x<2>, x<3>),
                    list(x<0>, x<1>, x<2>, x<3>)
                ));
            }

            // cons
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        cons(x<0>, list()),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        cons(x<0>, list(x<1>)),
                        list(x<0>, x<1>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        cons(x<0>, list(x<1>, x<2>)),
                        list(x<0>, x<1>, x<2>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        cons(x<0>, list(x<1>, x<2>, x<3>)),
                        list(x<0>, x<1>, x<2>, x<3>)
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        cons(1, list()), list(1)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        cons(1, list('2')), list(1, '2')
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        cons(1, list('2', 3.3)), list(1, '2', 3.3)
                    ));
                }
            }

            // nil
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    nil<L>(), list()
                ));
            }

            // concat
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(), list()),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(), list(x<0>)),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(), list(x<0>, x<1>)),
                        list(x<0>, x<1>)
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(x<0>), list()),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(x<0>), list(x<1>)),
                        list(x<0>, x<1>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(x<0>), list(x<1>, x<2>)),
                        list(x<0>, x<1>, x<2>)
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(x<0>, x<1>), list()),
                        list(x<0>, x<1>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(x<0>, x<1>), list(x<2>)),
                        list(x<0>, x<1>, x<2>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(x<0>, x<1>), list(x<2>, x<3>)),
                        list(x<0>, x<1>, x<2>, x<3>)
                    ));
                }

                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        concat(list(), list()), list()
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        concat(list(1), list()), list(1)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        concat(list(), list(1)), list(1)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        concat(list(1), list('2')), list(1, '2')
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        concat(list(1, '2'), list(3.3)), list(1, '2', 3.3)
                    ));
                }
            }

            // init
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        init(list(undefined)),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        init(list(x<0>, undefined)),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        init(list(x<0>, x<1>, undefined)),
                        list(x<0>, x<1>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        init(list(x<0>, x<1>, x<2>, undefined)),
                        list(x<0>, x<1>, x<2>)
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        init(list(1, '2')), list(1)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        init(list(1, '2', 3.3)), list(1, '2')
                    ));
                }
            }

            // intersperse
            {
                auto z = x<999>;
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(), undefined),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(x<0>), undefined),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(x<0>, x<1>), z),
                    list(x<0>, z, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(x<0>, x<1>, x<2>), z),
                    list(x<0>, z, x<1>, z, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(x<0>, x<1>, x<2>, x<3>), z),
                    list(x<0>, z, x<1>, z, x<2>, z, x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(x<0>, x<1>, x<2>, x<3>, x<4>), z),
                    list(x<0>, z, x<1>, z, x<2>, z, x<3>, z, x<4>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>), z),
                    list(x<0>, z, x<1>, z, x<2>, z, x<3>, z, x<4>, z, x<5>)
                ));
            }

            // slice
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(), int_<0>, int_<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined), int_<0>, int_<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined, undefined), int_<0>, int_<0>),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(x<0>), int_<0>, int_<1>),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(x<0>, undefined), int_<0>, int_<1>),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined, x<1>), int_<1>, int_<2>),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined, x<1>, undefined), int_<1>, int_<2>),
                    list(x<1>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(x<0>, x<1>), int_<0>, int_<2>),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(x<0>, x<1>, undefined), int_<0>, int_<2>),
                    list(x<0>, x<1>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined, x<1>, x<2>), int_<1>, int_<3>),
                    list(x<1>, x<2>)
                ));
            }

            // snoc
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        snoc(list(), x<0>),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        snoc(list(x<0>), x<1>),
                        list(x<0>, x<1>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        snoc(list(x<0>, x<1>), x<2>),
                        list(x<0>, x<1>, x<2>)
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        snoc(list(), 1), list(1)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        snoc(list(1), '2'), list(1, '2')
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        snoc(list(1, '2'), 3.3), list(1, '2', 3.3)
                    ));
                }
            }

            // take
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<0>, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<1>, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<2>, list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<0>, list(x<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<1>, list(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<2>, list(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<3>, list(x<0>)),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<0>, list(x<0>, x<1>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<1>, list(x<0>, x<1>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<2>, list(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<3>, list(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(int_<10>, list(x<0>, x<1>, x<2>, x<3>,  x<4>,  x<5>,  x<6>,
                                        x<7>, x<8>, x<9>, x<10>, x<11>, x<12>, x<13>)),
                    list(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>)
                ));
            }

            // remove_at
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<0>, list(x<0>)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<0>, list(x<0>, x<1>)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<1>, list(x<0>, x<1>)),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<0>, list(x<0>, x<1>, x<2>)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<1>, list(x<0>, x<1>, x<2>)),
                    list(x<0>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<2>, list(x<0>, x<1>, x<2>)),
                    list(x<0>, x<1>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<0>, list(x<0>, x<1>, x<2>, x<3>)),
                    list(x<1>, x<2>, x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<1>, list(x<0>, x<1>, x<2>, x<3>)),
                    list(x<0>, x<2>, x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<2>, list(x<0>, x<1>, x<2>, x<3>)),
                    list(x<0>, x<1>, x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<3>, list(x<0>, x<1>, x<2>, x<3>)),
                    list(x<0>, x<1>, x<2>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<0>, list(x<0>, x<1>, x<2>, x<3>, x<4>)),
                    list(x<1>, x<2>, x<3>, x<4>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<1>, list(x<0>, x<1>, x<2>, x<3>, x<4>)),
                    list(x<0>, x<2>, x<3>, x<4>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<2>, list(x<0>, x<1>, x<2>, x<3>, x<4>)),
                    list(x<0>, x<1>, x<3>, x<4>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<3>, list(x<0>, x<1>, x<2>, x<3>, x<4>)),
                    list(x<0>, x<1>, x<2>, x<4>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at(int_<4>, list(x<0>, x<1>, x<2>, x<3>, x<4>)),
                    list(x<0>, x<1>, x<2>, x<3>)
                ));
            }

            // remove_at_c
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<0>(list(x<0>)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<0>(list(x<0>, x<1>)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<1>(list(x<0>, x<1>)),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<0>(list(x<0>, x<1>, x<2>)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<1>(list(x<0>, x<1>, x<2>)),
                    list(x<0>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<2>(list(x<0>, x<1>, x<2>)),
                    list(x<0>, x<1>)
                ));
            }

            // repeat
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    repeat<L>(int_<0>, x<0>),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    repeat<L>(int_<1>, x<0>),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    repeat<L>(int_<2>, x<0>),
                    list(x<0>, x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    repeat<L>(int_<3>, x<0>),
                    list(x<0>, x<0>, x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    repeat<L>(int_<4>, x<0>),
                    list(x<0>, x<0>, x<0>, x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    repeat<L>(int_<5>, x<0>),
                    list(x<0>, x<0>, x<0>, x<0>, x<0>)
                ));
            }

            // reverse
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        reverse(list()),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        reverse(list(x<0>)),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        reverse(list(x<0>, x<1>)),
                        list(x<1>, x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        reverse(list(x<0>, x<1>, x<2>)),
                        list(x<2>, x<1>, x<0>)
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        reverse(list(1)), list(1)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        reverse(list(1, '2')), list('2', 1)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        reverse(list(1, '2', 3.3)), list(3.3, '2', 1)
                    ));
                }
            }

            // sort
            {

                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<0>)),
                    list(ord<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<0>, ord<1>)),
                    list(ord<0>, ord<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<1>, ord<0>)),
                    list(ord<0>, ord<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<1>, ord<0>, ord<4>, ord<2>)),
                    list(ord<0>, ord<1>, ord<2>, ord<4>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<1>, ord<0>, ord<-4>, ord<2>)),
                    list(ord<-4>, ord<0>, ord<1>, ord<2>)
                ));
            }

            // unzip
            {
                auto t = list; // tests are unreadable otherwise. mnemonic: tuple

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t())),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(), t())),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(x<0>, x<2>), t(x<1>, x<3>, x<4>))),
                    list(t(x<0>, x<1>), t(x<2>, x<3>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(),     t(),     t()))    ,
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(x<0>), t(),     t()))    ,
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(),     t(x<1>), t()))    ,
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(),     t(),     t(x<2>))),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(x<0>), t(x<1>), t()))    ,
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(),     t(x<1>), t(x<2>))),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(x<0>), t(),     t(x<2>))),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(x<0>), t(x<1>), t(x<2>))),
                    list(t(x<0>, x<1>, x<2>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unzip(list(t(x<0>, x<3>), t(x<1>, x<4>), t(x<2>, x<5>))),
                    list(t(x<0>, x<1>, x<2>), t(x<3>, x<4>, x<5>))
                ));
            }

            // span
            {
                auto z = x<999>;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(), is(z)),
                    prod(list(), list())
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(x<0>), is(z)),
                    prod(list(), list(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z), is(z)),
                    prod(list(z), list())
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(x<0>, z), is(z)),
                    prod(list(), list(x<0>, z))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z, x<0>), is(z)),
                    prod(list(z), list(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(x<0>, x<1>), is(z)),
                    prod(list(), list(x<0>, x<1>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(x<0>, x<1>, x<2>), is(z)),
                    prod(list(), list(x<0>, x<1>, x<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z, x<1>, x<2>), is(z)),
                    prod(list(z), list(x<1>, x<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(x<0>, z, x<2>), is(z)),
                    prod(list(), list(x<0>, z, x<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z, z, x<2>), is(z)),
                    prod(list(z, z), list(x<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z, z, z), is(z)),
                    prod(list(z, z, z), list())
                ));
            }

            // take_while
            {
                auto z = x<999>;
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(), isnt(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(x<1>), isnt(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(z), isnt(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(x<1>, x<2>), isnt(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(x<1>, z), isnt(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(z, x<2>), isnt(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(x<1>, x<2>, x<3>), isnt(z)),
                    list(x<1>, x<2>, x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(x<1>, x<2>, z), isnt(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(x<1>, z, x<3>), isnt(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(z, x<2>, x<3>), isnt(z)),
                    list()
                ));
            }

            // take_until
            {
                auto z = x<999>;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(), is(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(x<1>), is(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(z), is(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(x<1>, x<2>), is(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(x<1>, z), is(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(z, x<2>), is(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(x<1>, x<2>, x<3>), is(z)),
                    list(x<1>, x<2>, x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(x<1>, x<2>, z), is(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_until(list(x<1>, z, x<3>), is(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
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

                        BOOST_HANA_CONSTANT_CHECK(and_(
                            equal(length(expected), length(actual)),
                            all(actual, [=](auto x) { return elem(expected, x); })
                        ));
                    };
                };

                BOOST_HANA_CONSTANT_CHECK(equal(permutations(list()), list(list())));

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

                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(), isnt(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(z), isnt(z)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(x<1>), isnt(z)),
                    list(x<1>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(x<1>, x<2>), isnt(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(z, x<2>), isnt(z)),
                    list(x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(x<1>, z), isnt(z)),
                    list(x<1>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(z, x<2>, x<3>), isnt(z)),
                    list(x<2>, x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(x<1>, z, x<3>), isnt(z)),
                    list(x<1>, x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(x<1>, x<2>, z), isnt(z)),
                    list(x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(x<1>, z, z), isnt(z)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(z, x<2>, z), isnt(z)),
                    list(x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    filter(list(z, z, x<3>), isnt(z)),
                    list(x<3>)
                ));
            }

            // group
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<0>)),
                    list(list(x<0>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<0>, x<0>)),
                    list(list(x<0>, x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<0>, x<1>)),
                    list(list(x<0>), list(x<1>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<0>, x<0>, x<0>)),
                    list(list(x<0>, x<0>, x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<0>, x<0>, x<1>)),
                    list(list(x<0>, x<0>), list(x<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<0>, x<1>, x<0>)),
                    list(list(x<0>), list(x<1>), list(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<1>, x<0>, x<0>)),
                    list(list(x<1>), list(x<0>, x<0>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<0>, x<0>, x<1>, x<1>)),
                    list(list(x<0>, x<0>), list(x<1>, x<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(x<0>, x<0>, x<1>, x<1>, x<2>, x<2>, x<2>)),
                    list(list(x<0>, x<0>), list(x<1>, x<1>), list(x<2>, x<2>, x<2>))
                ));
            }

            // partition
            {
                auto pred = [=](auto z) {
                    return elem(list(x<-1>, x<-2>, x<-3>, x<-4>, x<-5>), z);
                };

                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(), pred),
                    prod(list(), list())
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(x<0>), pred),
                    prod(list(), list(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(x<0>, x<1>), pred),
                    prod(list(), list(x<0>, x<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(x<-1>), pred),
                    prod(list(x<-1>), list())
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(x<-1>, x<0>, x<2>), pred),
                    prod(list(x<-1>), list(x<0>, x<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(x<0>, x<-3>, x<2>, x<-5>, x<6>), pred),
                    prod(list(x<-3>, x<-5>), list(x<0>, x<2>, x<6>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
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
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl(list(), s, f),
                        list(s)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl(list(x<0>), s, f),
                        list(s, f(s, x<0>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl(list(x<0>, x<1>), s, f),
                        list(s, f(s, x<0>), f(f(s, x<0>), x<1>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl(list(x<0>, x<1>, x<2>), s, f),
                        list(s, f(s, x<0>), f(f(s, x<0>), x<1>), f(f(f(s, x<0>), x<1>), x<2>))
                    ));
                }

                // scanr
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanr(list(), s, f),
                        list(s)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanr(list(x<0>), s, f),
                        list(f(x<0>, s), s)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanr(list(x<0>, x<1>), s, f),
                        list(f(x<0>, f(x<1>, s)), f(x<1>, s), s)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanr(list(x<0>, x<1>, x<2>), s, f),
                        list(f(x<0>, f(x<1>, f(x<2>, s))), f(x<1>, f(x<2>, s)), f(x<2>, s), s)
                    ));
                }

                // scanl1
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl1(list(), f),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl1(list(x<0>), f),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl1(list(x<0>, x<1>), f),
                        list(x<0>, f(x<0>, x<1>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl1(list(x<0>, x<1>, x<2>), f),
                        list(x<0>, f(x<0>, x<1>), f(f(x<0>, x<1>), x<2>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanl1(list(x<0>, x<1>, x<2>, x<3>), f),
                        list(x<0>, f(x<0>, x<1>), f(f(x<0>, x<1>), x<2>), f(f(f(x<0>, x<1>), x<2>), x<3>))
                    ));
                }

                // scanr1
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanr1(list(), f),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanr1(list(x<0>), f),
                        list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        scanr1(list(x<0>, x<1>), f),
                        list(f(x<0>, x<1>), x<1>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
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

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfoldl<L>(stop_at(int_<0>), int_<0>),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfoldl<L>(stop_at(int_<1>), int_<0>),
                        list(f(int_<0>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfoldl<L>(stop_at(int_<2>), int_<0>),
                        list(f(int_<1>), f(int_<0>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfoldl<L>(stop_at(int_<3>), int_<0>),
                        list(f(int_<2>), f(int_<1>), f(int_<0>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
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

                    BOOST_HANA_CONSTANT_CHECK( equal(
                        unfoldr<L>(stop_at(int_<0>), int_<0>),
                        list()
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfoldr<L>(stop_at(int_<1>), int_<0>),
                        list(f(int_<0>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfoldr<L>(stop_at(int_<2>), int_<0>),
                        list(f(int_<0>), f(int_<1>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfoldr<L>(stop_at(int_<3>), int_<0>),
                        list(f(int_<0>), f(int_<1>), f(int_<2>))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
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
                    BOOST_HANA_CONSTANT_CHECK(equal(g(z), nothing));
                    BOOST_HANA_CONSTANT_CHECK(equal(g(f(x<0>, z)), just(prod(x<0>, z))));
                    BOOST_HANA_CONSTANT_CHECK(equal(g(f(z, x<0>)), just(prod(z, x<0>))));

                    // Make sure the reversing works
                    for_each(list(list(), list(x<0>), list(x<0>, x<1>), list(x<0>, x<1>, x<2>)),
                        [=](auto xs) {
                            BOOST_HANA_CONSTANT_CHECK(equal(
                                unfoldl<L>(g, foldl(xs, z, f)),
                                xs
                            ));
                            BOOST_HANA_CONSTANT_CHECK(equal(
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
                    auto perms = transform(
                        permutations(list(a(sorted)...)),
                        partial(sort_by, pred)
                    );
                    BOOST_HANA_CONSTANT_CHECK(all(perms, [=](auto xs) {
                        return equal(xs, list(a(sorted)...));
                    }));
                };

                check();
                check(ord<1>);
                check(ord<1>, ord<2>);
                check(ord<1>, ord<2>, ord<3>);

                // check stability
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort_by(pred, list(a(ord<1>), b(ord<1>))),
                    list(a(ord<1>), b(ord<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort_by(pred, list(b(ord<1>), a(ord<1>))),
                    list(b(ord<1>), a(ord<1>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort_by(pred, list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))),
                    list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort_by(pred, list(a(ord<1>), a(ord<2>), b(ord<1>), b(ord<2>))),
                    list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort_by(pred, list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))),
                    list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort_by(pred, list(a(ord<2>), b(ord<1>), b(ord<2>), a(ord<1>))),
                    list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
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

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group_by(pred, list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group_by(pred, list(a(x<0>))),
                    list(list(a(x<0>)))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group_by(pred, list(a(x<0>), b(x<0>))),
                    list(list(a(x<0>), b(x<0>)))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group_by(pred, list(a(x<0>), b(x<0>), a(x<1>))),
                    list(list(a(x<0>), b(x<0>)), list(a(x<1>)))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group_by(pred, list(a(x<0>), b(x<0>), a(x<1>), b(x<1>))),
                    list(list(a(x<0>), b(x<0>)), list(a(x<1>), b(x<1>)))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group_by(pred, list(a(x<0>), b(x<0>), a(x<1>), b(x<1>), b(x<0>))),
                    list(list(a(x<0>), b(x<0>)), list(a(x<1>), b(x<1>)), list(b(x<0>)))
                ));
            }

            // zip
            {
                auto t = list; // tests are unreadable otherwise. mnemonic: tuple
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(x<0>)),
                    list(t(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(x<0>, x<1>)),
                    list(t(x<0>), t(x<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(x<0>, x<1>, x<2>)),
                    list(t(x<0>), t(x<1>), t(x<2>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(x<0>), list(x<1>)),
                    list(t(x<0>, x<1>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(x<0>), list(x<1>), list(x<2>)),
                    list(t(x<0>, x<1>, x<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(x<0>, x<3>), list(x<1>, x<4>), list(x<2>, x<5>, x<8>)),
                    list(t(x<0>, x<1>, x<2>), t(x<3>, x<4>, x<5>))
                ));
            }

            // zip_with
            {
                auto f = injection([]{});

                // zip 1
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<0>)),
                    list(f(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<0>, x<1>)),
                    list(f(x<0>), f(x<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<0>, x<1>, x<2>)),
                    list(f(x<0>), f(x<1>), f(x<2>))
                ));

                // zip 2
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(undefined), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(), list(undefined)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<1>), list(x<-1>)),
                    list(f(x<1>, x<-1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<1>, x<2>), list(x<-1>)),
                    list(f(x<1>, x<-1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<1>), list(x<-1>, x<-2>)),
                    list(f(x<1>, x<-1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<1>, x<2>), list(x<-1>, x<-2>)),
                    list(f(x<1>, x<-1>), f(x<2>, x<-2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<1>, x<2>, x<3>, x<4>), list(x<-1>, x<-2>, x<-3>)),
                    list(f(x<1>, x<-1>), f(x<2>, x<-2>), f(x<3>, x<-3>))
                ));

                // zip 3
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(),          list(),          list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(undefined), list(),          list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(),          list(undefined), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(),          list(),          list(undefined)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(),          list(undefined), list(undefined)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(undefined), list(),          list(undefined)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined, list(undefined), list(undefined), list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(x<0>), list(x<1>), list(x<2>)),
                    list(f(x<0>, x<1>, x<2>))
                ));

                // zip 4
                BOOST_HANA_CONSTANT_CHECK(equal(
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
                BOOST_HANA_CONSTANT_CHECK(equal(
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

            // transform
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        transform(list(), f),
                        list()
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        transform(list(x<1>), f),
                        list(f(x<1>))
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        transform(list(x<1>, x<2>), f),
                        list(f(x<1>), f(x<2>))
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        transform(list(x<1>, x<2>, x<3>), f),
                        list(f(x<1>), f(x<2>), f(x<3>))
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        transform(list(x<1>, x<2>, x<3>, x<4>), f),
                        list(f(x<1>), f(x<2>), f(x<3>), f(x<4>))
                    ));
                }

                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        transform(list(), f),
                        list()
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        transform(list(1), f),
                        list(f(1))
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        transform(list(1, '2'), f),
                        list(f(1), f('2'))
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        transform(list(1, '2', 3.3f), f),
                        list(f(1), f('2'), f(3.3f))
                    ));
                }
            }

            // replace
            {
                BOOST_HANA_CONSTEXPR_LAMBDA auto is_even = [](auto x) {
                    return x % 2 == 0;
                };

                BOOST_HANA_CONSTANT_CHECK(equal(
                    replace(list(), is_even, 'x'),
                    list()
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    replace(list(0), is_even, 'x'),
                    list('x')
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    replace(list(0, 1), is_even, 'x'),
                    list('x', 1)
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    replace(list(0, 1, 2), is_even, 'x'),
                    list('x', 1, 'x')
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    replace(list(0, 1, 2, 3), is_even, 'x'),
                    list('x', 1, 'x', 3)
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
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(), list(x<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(), list(x<0>, x<1>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(), list(x<0>, x<1>, x<2>)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(f), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(f), list(x<0>)),
                    list(f(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(f), list(x<0>, x<1>)),
                    list(f(x<0>), f(x<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(f), list(x<0>, x<1>, x<2>)),
                    list(f(x<0>), f(x<1>), f(x<2>))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(f, g), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(f, g), list(x<0>)),
                    list(f(x<0>), g(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(f, g), list(x<0>, x<1>)),
                    list(f(x<0>), f(x<1>), g(x<0>), g(x<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ap(list(f, g), list(x<0>, x<1>, x<2>)),
                    list(f(x<0>), f(x<1>), f(x<2>), g(x<0>), g(x<1>), g(x<2>))
                ));
            }

            // lift
            {
                BOOST_HANA_CONSTANT_CHECK(equal(lift<L>(x<0>), list(x<0>)));
                BOOST_HANA_CONSTANT_CHECK(equal(lift<L>(x<1>), list(x<1>)));
            }
        }

        // Monad instance
        {
            laws<Monad, L>();

            using test::x;

            // flatten
            {
                BOOST_HANA_CONSTANT_CHECK(
                    equal(flatten(list(list(), list())),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(
                    equal(flatten(list(list(x<0>), list())),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(
                    equal(flatten(list(list(), list(x<0>))),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(
                    equal(flatten(list(list(x<0>), list(x<1>))),
                    list(x<0>, x<1>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
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
                BOOST_HANA_CONSTANT_CHECK(equal(
                    traverse<A>(list(), compose(applicative, f)),
                    applicative(list())
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    traverse<A>(list(x<0>), compose(applicative, f)),
                    applicative(list(f(x<0>)))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    traverse<A>(list(x<0>, x<1>), compose(applicative, f)),
                    applicative(list(f(x<0>), f(x<1>)))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    traverse<A>(list(x<0>, x<1>, x<2>), compose(applicative, f)),
                    applicative(list(f(x<0>), f(x<1>), f(x<2>)))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    traverse<A>(list(x<0>, x<1>, x<2>, x<3>), compose(applicative, f)),
                    applicative(list(f(x<0>), f(x<1>), f(x<2>), f(x<3>)))
                ));
            }

            // sequence
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sequence<A>(list()),
                    applicative(list())
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sequence<A>(list(applicative(x<0>))),
                    applicative(list(x<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sequence<A>(list(applicative(x<0>), applicative(x<1>))),
                    applicative(list(x<0>, x<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
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
                BOOST_HANA_CONSTANT_CHECK(equal(
                    list(),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(not_(equal(
                    list(x<0>),
                    list()
                )));
                BOOST_HANA_CONSTANT_CHECK(not_(equal(
                    list(),
                    list(x<0>)
                )));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    list(x<0>),
                    list(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(not_(equal(
                    list(x<0>, x<1>),
                    list(x<0>)
                )));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    list(x<0>, x<1>),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(not_(equal(
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
            laws<Foldable, L>();

            auto f = test::injection([]{});
            using test::x;
            constexpr struct { } undefined{};

            // foldl
            {
                {
                    auto s = x<999>;
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        foldl(list(x<1>), s, f),
                        f(s, x<1>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        foldl(list(x<1>, x<2>), s, f),
                        f(f(s, x<1>), x<2>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        foldl(list(x<1>, x<2>, x<3>), s, f),
                        f(f(f(s, x<1>), x<2>), x<3>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        foldl(list(x<1>, x<2>, x<3>, x<4>), s, f),
                        f(f(f(f(s, x<1>), x<2>), x<3>), x<4>)
                    ));
                }

                {
                    constexpr auto s = 0;
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldl(list(), s, f),
                        s
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldl(list(1), s, f),
                        f(s, 1)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldl(list(1, '2'), s, f),
                        f(f(s, 1), '2')
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldl(list(1, '2', 3.3), s, f),
                        f(f(f(s, 1), '2'), 3.3)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldl(list(1, '2', 3.3, 4.4f), s, f),
                        f(f(f(f(s, 1), '2'), 3.3), 4.4f)
                    ));
                }
            }

            // foldl1
            {
                auto s = x<999>;
                BOOST_HANA_CONSTANT_CHECK(equal(
                    foldl1(list(s), f),
                    s
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    foldl1(list(s, x<2>), f),
                    f(s, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    foldl1(list(s, x<2>, x<3>), f),
                    f(f(s, x<2>), x<3>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    foldl1(list(s, x<2>, x<3>, x<4>), f),
                    f(f(f(s, x<2>), x<3>), x<4>)
                ));
            }

            // foldr
            {
                {
                    auto s = x<999>;
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        foldr(list(x<0>), s, f),
                        f(x<0>, s)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        foldr(list(x<0>, x<1>), s, f),
                        f(x<0>, f(x<1>, s))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        foldr(list(x<0>, x<1>, x<2>), s, f),
                        f(x<0>, f(x<1>, f(x<2>, s)))
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        foldr(list(x<0>, x<1>, x<2>, x<3>), s, f),
                        f(x<0>, f(x<1>, f(x<2>, f(x<3>, s))))
                    ));
                }

                {
                    constexpr auto s = 0;
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldr(list(), s, f),
                        s
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldr(list(1), s, f),
                        f(1, s)
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldr(list(1, '2'), s, f),
                        f(1, f('2', s))
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldr(list(1, '2', 3.3), s, f),
                        f(1, f('2', f(3.3, s)))
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        foldr(list(1, '2', 3.3, 4.4f), s, f),
                        f(1, f('2', f(3.3, f(4.4f, s))))
                    ));
                }
            }

            // foldr1
            {
                auto s = x<999>;
                BOOST_HANA_CONSTANT_CHECK(equal(
                    foldr1(list(s), f),
                    s
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    foldr1(list(x<1>, s), f),
                    f(x<1>, s)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    foldr1(list(x<1>, x<2>, s), f),
                    f(x<1>, f(x<2>, s))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
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
                    BOOST_HANA_RUNTIME_CHECK(seen == std::vector<int>{xs...});
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
                BOOST_HANA_CONSTANT_CHECK(equal(
                    length(list()), size_t<0>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    length(list(undefined)), size_t<1>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    length(list(undefined, undefined)), size_t<2>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    length(list(undefined, undefined, undefined)), size_t<3>
                ));

                int i = 0; // non-constexpr
                BOOST_HANA_CONSTANT_CHECK(equal(
                    length(list(i, i)),
                    size_t<2>
                ));
            }

            // maximum
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<0>)),
                        ord<0>
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<0>, ord<1>)),
                        ord<1>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<1>, ord<0>)),
                        ord<1>
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<0>, ord<1>, ord<2>)),
                        ord<2>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<1>, ord<0>, ord<2>)),
                        ord<2>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<1>, ord<2>, ord<0>)),
                        ord<2>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<2>, ord<1>, ord<0>)),
                        ord<2>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<2>, ord<0>, ord<1>)),
                        ord<2>
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<0>, ord<1>, ord<2>, ord<3>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<1>, ord<0>, ord<2>, ord<3>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<1>, ord<2>, ord<0>, ord<3>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<1>, ord<2>, ord<3>, ord<0>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<2>, ord<1>, ord<3>, ord<0>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<2>, ord<3>, ord<1>, ord<0>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<2>, ord<3>, ord<0>, ord<1>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<3>, ord<2>, ord<0>, ord<1>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<3>, ord<0>, ord<2>, ord<1>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<3>, ord<0>, ord<1>, ord<2>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<0>, ord<2>, ord<3>, ord<1>)),
                        ord<3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        maximum(list(ord<0>, ord<3>, ord<1>, ord<2>)),
                        ord<3>
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{0})),
                        int{0}
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{0}, char{1})),
                        char{1}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{1}, char{0})),
                        int{1}
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{0}, char{1}, long{2})),
                        long{2}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{1}, char{0}, long{2})),
                        long{2}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{1}, char{2}, long{0})),
                        char{2}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{2}, char{1}, long{0})),
                        int{2}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{2}, char{0}, long{1})),
                        int{2}
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{0}, char{1}, long{2}, double{3})),
                        double{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{1}, char{0}, long{2}, double{3})),
                        double{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{1}, char{2}, long{0}, double{3})),
                        double{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{1}, char{2}, long{3}, double{0})),
                        long{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{2}, char{1}, long{3}, double{0})),
                        long{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{2}, char{3}, long{1}, double{0})),
                        char{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{2}, char{3}, long{0}, double{1})),
                        char{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{3}, char{2}, long{0}, double{1})),
                        int{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{3}, char{0}, long{2}, double{1})),
                        int{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{3}, char{0}, long{1}, double{2})),
                        int{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{0}, char{2}, long{3}, double{1})),
                        long{3}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{0}, char{3}, long{1}, double{2})),
                        char{3}
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{0}, char{1}, long_<2>)),
                        long{2}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        maximum(list(int{0}, long_<1>, char{2})),
                        char{2}
                    ));
                }
            }

            // minimum
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<0>)),
                        ord<0>
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<0>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<1>, ord<0>)),
                        ord<0>
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<0>, ord<1>, ord<2>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<1>, ord<0>, ord<2>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<1>, ord<2>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<2>, ord<1>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<2>, ord<0>, ord<1>)),
                        ord<0>
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<0>, ord<1>, ord<2>, ord<3>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<1>, ord<0>, ord<2>, ord<3>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<1>, ord<2>, ord<0>, ord<3>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<1>, ord<2>, ord<3>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<2>, ord<1>, ord<3>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<2>, ord<3>, ord<1>, ord<0>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<2>, ord<3>, ord<0>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<3>, ord<2>, ord<0>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<3>, ord<0>, ord<2>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<3>, ord<0>, ord<1>, ord<2>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<0>, ord<2>, ord<3>, ord<1>)),
                        ord<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        minimum(list(ord<0>, ord<3>, ord<1>, ord<2>)),
                        ord<0>
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{0})),
                        int{0}
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{0}, char{1})),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{1}, char{0})),
                        char{0}
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{0}, char{1}, long{2})),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{1}, char{0}, long{2})),
                        char{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{1}, char{2}, long{0})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{2}, char{1}, long{0})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{2}, char{0}, long{1})),
                        char{0}
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{0}, char{1}, long{2}, double{3})),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{1}, char{0}, long{2}, double{3})),
                        char{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{1}, char{2}, long{0}, double{3})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{1}, char{2}, long{3}, double{0})),
                        double{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{2}, char{1}, long{3}, double{0})),
                        double{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{2}, char{3}, long{1}, double{0})),
                        double{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{2}, char{3}, long{0}, double{1})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{3}, char{2}, long{0}, double{1})),
                        long{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{3}, char{0}, long{2}, double{1})),
                        char{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{3}, char{0}, long{1}, double{2})),
                        char{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{0}, char{2}, long{3}, double{1})),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{0}, char{3}, long{1}, double{2})),
                        int{0}
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(int{0}, char{1}, long_<2>)),
                        int{0}
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        minimum(list(ord<0>, char{1}, long{2})),
                        ord<0>
                    ));
                }
            }

            // count
            {
                constexpr auto f = id;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(), f), size_t<0>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<1>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<0>), f), size_t<0>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<1>, char_<1>), f), size_t<2>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<1>, char_<0>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<0>, char_<1>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<0>, char_<0>), f), size_t<0>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<1>, char_<1>, long_<1>), f), size_t<3>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<1>, char_<1>, long_<0>), f), size_t<2>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<1>, char_<0>, long_<1>), f), size_t<2>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<1>, char_<0>, long_<0>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<0>, char_<1>, long_<1>), f), size_t<2>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<0>, char_<1>, long_<0>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<0>, char_<0>, long_<1>), f), size_t<1>
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    count(list(int_<0>, char_<0>, long_<0>), f), size_t<0>
                ));


                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{1}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{0}), f), 0u
                ));

                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{1}, char{1}), f), 2u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{1}, char{0}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{0}, char{1}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{0}, char{0}), f), 0u
                ));

                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{1}, char{1}, double{1}), f), 3u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{1}, char{1}, double{0}), f), 2u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{1}, char{0}, double{1}), f), 2u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{1}, char{0}, double{0}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{0}, char{1}, double{1}), f), 2u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{0}, char{1}, double{0}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{0}, char{0}, double{1}), f), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(int{0}, char{0}, double{0}), f), 0u
                ));


                BOOST_HANA_CONSTEXPR_LAMBDA auto is_even = [](auto x) {
                    return x % 2 == 0;
                };
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(), is_even), 0u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(1), is_even), 0u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(2), is_even), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(1, 2), is_even), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(1, 2, 3), is_even), 1u
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    count(list(1, 2, 3, 4), is_even), 2u
                ));
            }

            // product
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        product(list()),
                        int_<1>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        product(list(int_<2>)),
                        int_<2>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        product(list(int_<2>, int_<3>)),
                        int_<2 * 3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        product(list(int_<2>, int_<3>, int_<4>)),
                        int_<2 * 3 * 4>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        product(list(int_<2>, int_<3>, int_<4>, int_<5>)),
                        int_<2 * 3 * 4 * 5>
                    ));
                }
                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        product(list(2)),
                        2
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        product(list(2, 3)),
                        2 * 3
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        product(list(2, 3, 4)),
                        2 * 3 * 4
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        product(list(2, 3, 4, 5)),
                        2 * 3 * 4 * 5
                    ));
                }
            }

            // sum
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        sum(list()),
                        int_<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        sum(list(int_<1>)),
                        int_<1>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        sum(list(int_<1>, int_<2>)),
                        int_<1 + 2>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        sum(list(int_<1>, int_<2>, int_<3>)),
                        int_<1 + 2 + 3>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        sum(list(int_<1>, int_<2>, int_<3>, int_<4>)),
                        int_<1 + 2 + 3 + 4>
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        sum(list(1)), 1
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        sum(list(1, 2)), 1 + 2
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        sum(list(1, 2, 3)), 1 + 2 + 3
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        sum(list(1, 2, 3, 4)), 1 + 2 + 3 + 4
                    ));
                }
            }

            // unpack
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unpack(list(), f),
                    f()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unpack(list(x<0>), f),
                    f(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unpack(list(x<0>, x<1>), f),
                    f(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unpack(list(x<0>, x<1>, x<2>), f),
                    f(x<0>, x<1>, x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
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
            laws<Searchable, L>();

            BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
                return [=](auto y) { return equal(x, y); };
            };

            BOOST_HANA_CONSTEXPR_LAMBDA auto is_even = [](auto x) {
                return x % 2 == 0;
            };

            auto c = test::numeric; // a minimal comparable
            auto logical = test::numeric;

            // any
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(not_(any(list(), is(x<9>))));

                    BOOST_HANA_CONSTANT_CHECK(not_(any(list(x<0>), is(x<9>))));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>), is(x<0>)));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>, invalid<1>), is(x<0>)));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>, invalid<1>, invalid<2>), is(x<0>)));

                    BOOST_HANA_CONSTANT_CHECK(not_(any(list(x<0>, x<1>), is(x<9>))));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>, x<1>), is(x<1>)));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>, x<1>, invalid<2>), is(x<1>)));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>, x<1>, invalid<2>, invalid<3>), is(x<1>)));

                    BOOST_HANA_CONSTANT_CHECK(not_(any(list(x<0>, x<1>, x<2>), is(x<9>))));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>, x<1>, x<2>), is(x<2>)));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>, x<1>, x<2>, nothing), is(x<2>)));
                    BOOST_HANA_CONSTANT_CHECK(any(list(x<0>, x<1>, x<2>, nothing, nothing), is(x<2>)));
                }

                {
                    BOOST_HANA_CONSTANT_CHECK(not_(
                        any(list(), undefined)
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(
                        any(list(c(0)), is(c(0)))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(
                        not_(any(list(c(0)), is(c(1))))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(not_(any(list(1), is_even)));
                    BOOST_HANA_CONSTEXPR_CHECK(any(list(2), is_even));
                    BOOST_HANA_CONSTEXPR_CHECK(any(list(1, 2), is_even));
                    BOOST_HANA_CONSTEXPR_CHECK(not_(any(list(1, 3), is_even)));
                    BOOST_HANA_CONSTEXPR_CHECK(any(list(1, 3, 4), is_even));
                }
            }

            // any_of
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(not_(
                        any_of(list())
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(
                        any_of(list(logical(true)))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(
                        not_(any_of(list(logical(false))))
                    );
                }
            }

            // all
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(all(list(), undefined));
                    BOOST_HANA_CONSTEXPR_CHECK(
                        all(list(c(0)), is(c(0)))
                    );
                    BOOST_HANA_CONSTEXPR_CHECK(
                        not_(all(list(c(0)), is(c(1))))
                    );
                    BOOST_HANA_CONSTEXPR_CHECK(not_(
                        all(list(c(0), c(1)), is(c(0)))
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(
                        all(list(c(0), c(0)), is(c(0)))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(not_(all(list(1), is_even)));
                    BOOST_HANA_CONSTEXPR_CHECK(all(list(2), is_even));
                    BOOST_HANA_CONSTEXPR_CHECK(all(list(2, 4), is_even));
                    BOOST_HANA_CONSTEXPR_CHECK(not_(all(list(1, 2), is_even)));
                    BOOST_HANA_CONSTEXPR_CHECK(not_(all(list(1, 3), is_even)));
                    BOOST_HANA_CONSTEXPR_CHECK(not_(all(list(1, 3, 4), is_even)));
                }
            }

            // all_of
            {
                BOOST_HANA_CONSTANT_CHECK(
                    all_of(list())
                );

                BOOST_HANA_CONSTEXPR_CHECK(
                    all_of(list(logical(true)))
                );

                BOOST_HANA_CONSTEXPR_CHECK(
                    not_(all_of(list(logical(false))))
                );

                BOOST_HANA_CONSTEXPR_CHECK(
                    all_of(list(logical(true), logical(true)))
                );

                BOOST_HANA_CONSTEXPR_CHECK(not_(
                    all_of(list(logical(true), logical(false)))
                ));
            }

            // none
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(none(list(), undefined));

                    BOOST_HANA_CONSTEXPR_CHECK(
                        none(list(c(0)), is(c(1)))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(
                        not_(none(list(c(0)), is(c(0))))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(none(list(1), is_even));
                    BOOST_HANA_CONSTEXPR_CHECK(not_(none(list(2), is_even)));
                    BOOST_HANA_CONSTEXPR_CHECK(not_(none(list(1, 2), is_even)));
                    BOOST_HANA_CONSTEXPR_CHECK(none(list(1, 3), is_even));
                    BOOST_HANA_CONSTEXPR_CHECK(not_(none(list(1, 3, 4), is_even)));
                }
            }

            // none_of
            {
                // runtime
                {
                    BOOST_HANA_CONSTANT_CHECK(none_of(list()));

                    BOOST_HANA_CONSTEXPR_CHECK(
                        none_of(list(logical(false)))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(
                        not_(none_of(list(logical(true))))
                    );
                }
            }

            // find
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(), is(x<9>)),
                    nothing
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>), is(x<9>)),
                    nothing
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>), is(x<0>)),
                    just(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, invalid<1>), is(x<0>)),
                    just(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, invalid<1>, invalid<2>), is(x<0>)),
                    just(x<0>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, x<1>), is(x<9>)),
                    nothing
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, x<1>), is(x<1>)),
                    just(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, x<1>, invalid<2>), is(x<1>)),
                    just(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, x<1>, invalid<2>, invalid<3>), is(x<1>)),
                    just(x<1>)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, x<1>, x<2>), is(x<9>)),
                    nothing
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, x<1>, x<2>), is(x<2>)),
                    just(x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, x<1>, x<2>, nothing), is(x<2>)),
                    just(x<2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    find(list(x<0>, x<1>, x<2>, nothing, nothing), is(x<2>)),
                    just(x<2>)
                ));
            }

            // elem
            {
                // runtime
                {
                    BOOST_HANA_CONSTEXPR_CHECK(
                        elem(list(c(0)), c(0))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(
                        not_(elem(list(c(0)), c(1)))
                    );
                }
            }

            // in
            {
                // runtime
                {
                    BOOST_HANA_CONSTEXPR_CHECK(
                        c(0) ^in^ list(c(0))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(not_(
                        c(1) ^in^ list(c(0))
                    ));
                }
            }

            // subset
            {
                // runtime
                {
                    BOOST_HANA_CONSTANT_CHECK(subset(
                        list(), list(c(1)))
                    );

                    BOOST_HANA_CONSTEXPR_CHECK(subset(
                        list(c(0)), list(c(0))
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(subset(
                        list(c(0)), list(c(0), c(1))
                    ));

                    BOOST_HANA_CONSTEXPR_CHECK(not_(subset(
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
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        head(list(x<0>)),
                        x<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        head(list(x<0>, invalid<>)),
                        x<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        head(list(x<0>, invalid<1>, invalid<2>)),
                        x<0>
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        head(list(1)), 1
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        head(list(1, '2')), 1
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        head(list(1, '2', 3.3)), 1
                    ));
                }
            }

            // is_empty
            {
                BOOST_HANA_CONSTANT_CHECK(is_empty(list()));
                BOOST_HANA_CONSTANT_CHECK(not_(is_empty(list(invalid<>))));
                BOOST_HANA_CONSTANT_CHECK(not_(is_empty(list(invalid<0>, invalid<1>))));

                int i = 0; // non-constexpr
                BOOST_HANA_CONSTANT_CHECK(not_(is_empty(list(i))));
                BOOST_HANA_CONSTANT_CHECK(not_(is_empty(list(i, i))));
            }

            // tail
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        tail(list(invalid<>, x<0>)), list(x<0>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        tail(list(invalid<>, x<0>, x<1>)), list(x<0>, x<1>)
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        tail(list(invalid<>, x<0>, x<1>, x<2>)), list(x<0>, x<1>, x<2>)
                    ));
                }

                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        tail(list(1)), list()
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        tail(list(1, '2')), list('2')
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        tail(list(1, '2', 3.3)), list('2', 3.3)
                    ));
                }
            }

            // at
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        at(size_t<0>, list(x<0>)), x<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        at(size_t<0>, list(x<0>, invalid<>)), x<0>
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        at(size_t<1>, list(invalid<>, x<1>)), x<1>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        at(size_t<1>, list(invalid<0>, x<1>, invalid<2>)), x<1>
                    ));

                    BOOST_HANA_CONSTANT_CHECK(equal(
                        at(size_t<2>, list(invalid<0>, invalid<1>, x<2>)), x<2>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        at(size_t<2>, list(invalid<0>, invalid<1>, x<2>, invalid<3>)), x<2>
                    ));
                }

                // make sure we can use non-pods on both sides
                at(size_t<1>, list(non_pod<0>, x<1>, non_pod<2>));

                {
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
                }
            }

            // last
            {
                {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        last(list(x<0>)),
                        x<0>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        last(list(invalid<0>, x<1>)),
                        x<1>
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        last(list(invalid<0>, invalid<1>, x<2>)),
                        x<2>
                    ));
                }

                {
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        last(list(1)), 1
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        last(list(1, '2')), '2'
                    ));
                    BOOST_HANA_CONSTEXPR_CHECK(equal(
                        last(list(1, '2', 3.3)), 3.3
                    ));
                }
            }

            // drop
            {
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
                    drop(size_t<0>, list(x<0>)),
                    list(x<0>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop(size_t<1>, list(x<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop(size_t<2>, list(x<0>)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop(size_t<0>, list(x<0>, x<1>)),
                    list(x<0>, x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop(size_t<1>, list(x<0>, x<1>)),
                    list(x<1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop(size_t<2>, list(x<0>, x<1>)),
                    list()
                ));
            }

            // drop_until
            {
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
            }

            // drop_while
            {
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
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_LIST_HPP
