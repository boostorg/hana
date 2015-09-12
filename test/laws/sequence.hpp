/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_SEQUENCE_HPP
#define BOOST_HANA_TEST_LAWS_SEQUENCE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/functional/capture.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>

#include <test/equivalence_class.hpp>
#include <test/identity.hpp>
#include <test/minimal_product.hpp>
#include <test/numeric.hpp>
#include <test/seq.hpp>

#include <type_traits>
#include <vector>


#if defined(BOOST_HANA_TEST_SEQUENCE_KNOWN_PARTS) && \
    BOOST_HANA_TEST_SEQUENCE_KNOWN_PARTS != 3
#   error The Sequence laws are currently split into 3 different parts. \
          Your unit test must have gone out of sync.
#endif

#if defined(BOOST_HANA_TEST_SEQUENCE_PART) && \
    !defined(BOOST_HANA_TEST_SEQUENCE_KNOWN_PARTS)
#   error When splitting the Sequence test into subparts, you must define \
          the BOOST_HANA_TEST_SEQUENCE_KNOWN_PARTS macro to the number of \
          parts in the Sequence test to make sure your tests stay in sync.
#endif

#if defined(BOOST_HANA_TEST_SEQUENCE_PART) && BOOST_HANA_TEST_SEQUENCE_PART > 3
#   error BOOST_HANA_TEST_SEQUENCE_PART must not be defined or              \
          it must be a number in {1, 2, 3}
#endif

namespace boost { namespace hana { namespace test {
    template <typename S, typename = when<true>>
    struct TestSequence : TestSequence<S, laws> {
        using TestSequence<S, laws>::TestSequence;
    };

    template <typename S>
    struct TestSequence<S, laws> {
        template <int i>
        using eq = integer<i,
              Policy::Comparable
            | Policy::Constant
        >;

        template <int i>
        using cx_eq = integer<i,
              Policy::Comparable
            | Policy::Constexpr
        >;

        template <int i>
        using ord = integer<i,
              Policy::Orderable
            | Policy::Constant
        >;

        struct undefined { };

        TestSequence() {
            constexpr auto list = make<S>;
            constexpr auto foldable = seq; (void)foldable;

#if !defined(BOOST_HANA_TEST_SEQUENCE_PART) || BOOST_HANA_TEST_SEQUENCE_PART == 1
            //////////////////////////////////////////////////////////////////
            // Check for Sequence<...>
            //////////////////////////////////////////////////////////////////
            static_assert(Sequence<decltype(list())>::value, "");
            static_assert(Sequence<decltype(list(1))>::value, "");
            static_assert(Sequence<decltype(list(1, '2'))>::value, "");
            static_assert(Sequence<decltype(list(1, '2', 3.4))>::value, "");

            //////////////////////////////////////////////////////////////////
            // Check for basic tag consistency
            //////////////////////////////////////////////////////////////////
            struct Random;
            static_assert(std::is_same<tag_of_t<decltype(list())>, S>{}, "");
            static_assert(std::is_same<tag_of_t<decltype(list(1))>, S>{}, "");
            static_assert(std::is_same<tag_of_t<decltype(list(1, '2'))>, S>{}, "");
            static_assert(std::is_same<tag_of_t<decltype(list(1, '2', 3.3))>, S>{}, "");
            static_assert(!std::is_same<tag_of_t<Random>, S>{}, "");

            //////////////////////////////////////////////////////////////////
            // Foldable -> Sequence conversion
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<S>(foldable()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<S>(foldable(eq<0>{})),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<S>(foldable(eq<0>{}, eq<1>{})),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<S>(foldable(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(eq<0>{}, eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<S>(foldable(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})),
                    list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // make (tautological given our definition of `list`)
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<S>(),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<S>(eq<0>{}),
                list(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<S>(eq<0>{}, eq<1>{}),
                list(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<S>(eq<0>{}, eq<1>{}, eq<2>{}),
                list(eq<0>{}, eq<1>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<S>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}),
                list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
            ));

            //////////////////////////////////////////////////////////////////
            // insert
            //////////////////////////////////////////////////////////////////
            {
                auto z = eq<999>{};

                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}), size_c<0>, z),
                    list(z, eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}), size_c<1>, z),
                    list(eq<0>{}, z)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}), size_c<0>, z),
                    list(z, eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}), size_c<1>, z),
                    list(eq<0>{}, z, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}), size_c<2>, z),
                    list(eq<0>{}, eq<1>{}, z)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<0>, z),
                    list(z, eq<0>{}, eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<1>, z),
                    list(eq<0>{}, z, eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<2>, z),
                    list(eq<0>{}, eq<1>{}, z, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<3>, z),
                    list(eq<0>{}, eq<1>{}, eq<2>{}, z)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<0>, z),
                    list(z, eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<1>, z),
                    list(eq<0>{}, z, eq<1>{}, eq<2>{}, eq<3>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<2>, z),
                    list(eq<0>{}, eq<1>{}, z, eq<2>{}, eq<3>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<3>, z),
                    list(eq<0>{}, eq<1>{}, eq<2>{}, z, eq<3>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<4>, z),
                    list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, z)
                ));
            }

            //////////////////////////////////////////////////////////////////
            // insert_range
            //////////////////////////////////////////////////////////////////
            {
                auto foldable = test::seq;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}), size_c<0>,
                                 foldable()),
                    list(eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}), size_c<0>,
                                 foldable(eq<-1>{})),
                    list(eq<-1>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}), size_c<0>,
                                 foldable(eq<-1>{}, eq<-2>{})),
                    list(eq<-1>{}, eq<-2>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}), size_c<0>,
                                 foldable(eq<-1>{}, eq<-2>{}, eq<-3>{})),
                    list(eq<-1>{}, eq<-2>{}, eq<-3>{}, eq<1>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}, eq<2>{}), size_c<0>,
                                 foldable()),
                    list(eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}, eq<2>{}), size_c<0>,
                                 foldable(eq<-1>{})),
                    list(eq<-1>{}, eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}, eq<2>{}), size_c<0>,
                                 foldable(eq<-1>{}, eq<-2>{})),
                    list(eq<-1>{}, eq<-2>{}, eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}, eq<2>{}), size_c<1>,
                                 foldable()),
                    list(eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}, eq<2>{}), size_c<1>,
                                 foldable(eq<-1>{})),
                    list(eq<1>{}, eq<-1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}, eq<2>{}), size_c<1>,
                                 foldable(eq<-1>{}, eq<-2>{})),
                    list(eq<1>{}, eq<-1>{}, eq<-2>{}, eq<2>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    insert_range(list(eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}), size_c<2>,
                                 foldable(eq<-1>{}, eq<-2>{})),
                    list(eq<1>{}, eq<2>{}, eq<-1>{}, eq<-2>{}, eq<3>{}, eq<4>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // intersperse
            //////////////////////////////////////////////////////////////////
            {
                auto z = eq<999>{};

                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(), undefined{}),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(eq<0>{}), undefined{}),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(eq<0>{}, eq<1>{}), z),
                    list(eq<0>{}, z, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(eq<0>{}, eq<1>{}, eq<2>{}), z),
                    list(eq<0>{}, z, eq<1>{}, z, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), z),
                    list(eq<0>{}, z, eq<1>{}, z, eq<2>{}, z, eq<3>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}), z),
                    list(eq<0>{}, z, eq<1>{}, z, eq<2>{}, z, eq<3>{}, z, eq<4>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    intersperse(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}, eq<5>{}), z),
                    list(eq<0>{}, z, eq<1>{}, z, eq<2>{}, z, eq<3>{}, z, eq<4>{}, z, eq<5>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // slice (arbitrary indices)
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(), foldable()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}), foldable()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}, undefined{}), foldable()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}), foldable(size_c<0>)),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, undefined{}), foldable(size_c<0>)),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}, eq<1>{}), foldable(size_c<1>)),
                    list(eq<1>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}), foldable(size_c<0>, size_c<1>)),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}), foldable(size_c<1>, size_c<0>)),
                    list(eq<1>{}, eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}), foldable(size_c<0>, size_c<0>)),
                    list(eq<0>{}, eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}), foldable(size_c<1>, size_c<1>)),
                    list(eq<1>{}, eq<1>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}, eq<2>{}),
                          foldable(size_c<0>, size_c<1>, size_c<2>)),
                    list(eq<0>{}, eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}, eq<2>{}),
                          foldable(size_c<0>, size_c<2>, size_c<1>)),
                    list(eq<0>{}, eq<2>{}, eq<1>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}, eq<2>{}),
                          foldable(size_c<0>, size_c<2>, size_c<1>, size_c<0>)),
                    list(eq<0>{}, eq<2>{}, eq<1>{}, eq<0>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}, eq<2>{}),
                          foldable(size_c<0>, size_c<2>, size_c<1>, size_c<0>, size_c<1>)),
                    list(eq<0>{}, eq<2>{}, eq<1>{}, eq<0>{}, eq<1>{})
                ));

                // Try with a tuple_c
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}),
                          tuple_c<unsigned, 1, 3, 2>),
                    list(eq<1>{}, eq<3>{}, eq<2>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // With a `range` (check the optimization for contiguous indices)
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(), make_range(size_c<0>, size_c<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}), make_range(size_c<0>, size_c<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}, undefined{}), make_range(size_c<0>, size_c<0>)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}), make_range(size_c<0>, size_c<1>)),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, undefined{}), make_range(size_c<0>, size_c<1>)),
                    list(eq<0>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}, eq<1>{}), make_range(size_c<1>, size_c<2>)),
                    list(eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}, eq<1>{}, undefined{}), make_range(size_c<1>, size_c<2>)),
                    list(eq<1>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}), make_range(size_c<0>, size_c<2>)),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(eq<0>{}, eq<1>{}, undefined{}), make_range(size_c<0>, size_c<2>)),
                    list(eq<0>{}, eq<1>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}, eq<1>{}, eq<2>{}), make_range(size_c<1>, size_c<3>)),
                    list(eq<1>{}, eq<2>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    slice(list(undefined{}, eq<1>{}, eq<2>{}, undefined{}), make_range(size_c<1>, size_c<3>)),
                    list(eq<1>{}, eq<2>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // take
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(), size_c<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(), size_c<1>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(), size_c<2>),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}), size_c<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}), size_c<1>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}), size_c<2>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}), size_c<3>),
                    list(eq<0>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}, eq<1>{}), size_c<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}, eq<1>{}), size_c<1>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}, eq<1>{}), size_c<2>),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}, eq<1>{}), size_c<3>),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{},  eq<4>{},  eq<5>{},  eq<6>{},
                              eq<7>{}, eq<8>{}, eq<9>{}, eq<10>{}, eq<11>{}, eq<12>{}, eq<13>{}),
                        size_c<10>),
                    list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}, eq<5>{}, eq<6>{}, eq<7>{}, eq<8>{}, eq<9>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // remove_at
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}), size_c<0>),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}), size_c<0>),
                list(eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}), size_c<1>),
                list(eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<0>),
                list(eq<1>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<1>),
                list(eq<0>{}, eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<2>),
                list(eq<0>{}, eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<0>),
                list(eq<1>{}, eq<2>{}, eq<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<1>),
                list(eq<0>{}, eq<2>{}, eq<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<2>),
                list(eq<0>{}, eq<1>{}, eq<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<3>),
                list(eq<0>{}, eq<1>{}, eq<2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}), size_c<0>),
                list(eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}), size_c<1>),
                list(eq<0>{}, eq<2>{}, eq<3>{}, eq<4>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}), size_c<2>),
                list(eq<0>{}, eq<1>{}, eq<3>{}, eq<4>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}), size_c<3>),
                list(eq<0>{}, eq<1>{}, eq<2>{}, eq<4>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                remove_at(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}), size_c<4>),
                list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
            ));

            //////////////////////////////////////////////////////////////////
            // remove_at_c
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<0>(list(eq<0>{})),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<0>(list(eq<0>{}, eq<1>{})),
                    list(eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<1>(list(eq<0>{}, eq<1>{})),
                    list(eq<0>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<0>(list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<1>(list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(eq<0>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_at_c<2>(list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(eq<0>{}, eq<1>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // remove_range
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(), size_c<0>, size_c<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(), size_c<1>, size_c<1>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(), size_c<2>, size_c<2>),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}), size_c<0>, size_c<0>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}), size_c<0>, size_c<1>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}), size_c<1>, size_c<1>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}), size_c<2>, size_c<2>),
                    list(eq<0>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}), size_c<0>, size_c<0>),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}), size_c<0>, size_c<1>),
                    list(eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}), size_c<1>, size_c<2>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}), size_c<0>, size_c<2>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}), size_c<2>, size_c<2>),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}), size_c<9999>, size_c<9999>),
                    list(eq<0>{}, eq<1>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<0>, size_c<2>),
                    list(eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<1>, size_c<3>),
                    list(eq<0>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<0>, size_c<2>),
                    list(eq<2>{}, eq<3>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), size_c<2>, size_c<3>),
                    list(eq<0>{}, eq<1>{}, eq<3>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{},
                                      eq<5>{}, eq<6>{}, eq<7>{}, eq<8>{}, eq<9>{}),
                                 size_c<4>, size_c<7>),
                    list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<7>{}, eq<8>{}, eq<9>{})
                ));

                // remove_range_c
                BOOST_HANA_CONSTANT_CHECK(equal(
                    remove_range_c<4, 7>(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{},
                                              eq<5>{}, eq<6>{}, eq<7>{}, eq<8>{}, eq<9>{})),
                    list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<7>{}, eq<8>{}, eq<9>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // reverse
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    reverse(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    reverse(list(eq<0>{})),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    reverse(list(eq<0>{}, eq<1>{})),
                    list(eq<1>{}, eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    reverse(list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(eq<2>{}, eq<1>{}, eq<0>{})
                ));


                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    reverse(list(cx_eq<1>{})),
                    list(cx_eq<1>{})
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    reverse(list(cx_eq<1>{}, cx_eq<2>{})),
                    list(cx_eq<2>{}, cx_eq<1>{})
                ));
                BOOST_HANA_CONSTEXPR_CHECK(equal(
                    reverse(list(cx_eq<1>{}, cx_eq<2>{}, cx_eq<3>{})),
                    list(cx_eq<3>{}, cx_eq<2>{}, cx_eq<1>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // unique (with a predicate)
            //////////////////////////////////////////////////////////////////
            {
                auto a = [](auto z) { return test::tag(eq<999>{}, z); };
                auto b = [](auto z) { return test::tag(eq<888>{}, z); };
                auto c = [](auto z) { return test::tag(eq<777>{}, z); };

                BOOST_HANA_CONSTEXPR_LAMBDA auto pred = [](auto x, auto y) {
                    return equal(x.unwrap, y.unwrap);
                };

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(), undefined{}),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{})), pred),
                    list(a(eq<0>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{}), b(eq<0>{})), pred),
                    list(a(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{}), a(eq<1>{})), pred),
                    list(a(eq<0>{}), a(eq<1>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{}), b(eq<0>{}), c(eq<0>{})), pred),
                    list(a(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{}), b(eq<0>{}), c(eq<1>{})), pred),
                    list(a(eq<0>{}), c(eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{}), a(eq<1>{}), a(eq<0>{})), pred),
                    list(a(eq<0>{}), a(eq<1>{}), a(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{}), b(eq<1>{}), b(eq<1>{})), pred),
                    list(a(eq<0>{}), b(eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{}), a(eq<1>{}), a(eq<2>{})), pred),
                    list(a(eq<0>{}), a(eq<1>{}), a(eq<2>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(a(eq<0>{}), b(eq<0>{}),
                                a(eq<1>{}),
                                a(eq<2>{}), b(eq<2>{}), c(eq<2>{}),
                                a(eq<3>{}), b(eq<3>{}), c(eq<3>{}),
                                a(eq<0>{})), pred),
                    list(a(eq<0>{}), a(eq<1>{}), a(eq<2>{}), a(eq<3>{}), a(eq<0>{}))
                ));

                // unique.by
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique.by(pred, list(a(eq<0>{}), b(eq<0>{}),
                                         a(eq<1>{}),
                                         a(eq<2>{}), b(eq<2>{}), c(eq<2>{}),
                                         a(eq<3>{}), b(eq<3>{}), c(eq<3>{}),
                                         a(eq<0>{}))),
                    list(a(eq<0>{}), a(eq<1>{}), a(eq<2>{}), a(eq<3>{}), a(eq<0>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique.by(pred)(list(a(eq<0>{}), b(eq<0>{}),
                                         a(eq<1>{}),
                                         a(eq<2>{}), b(eq<2>{}), c(eq<2>{}),
                                         a(eq<3>{}), b(eq<3>{}), c(eq<3>{}),
                                         a(eq<0>{}))),
                    list(a(eq<0>{}), a(eq<1>{}), a(eq<2>{}), a(eq<3>{}), a(eq<0>{}))
                ));
            }

            //////////////////////////////////////////////////////////////////
            // unique (without a predicate)
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{})),
                    list(eq<0>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{}, eq<0>{})),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{}, eq<1>{})),
                    list(eq<0>{}, eq<1>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{}, eq<0>{}, eq<0>{})),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{}, eq<0>{}, eq<1>{})),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{}, eq<1>{}, eq<0>{})),
                    list(eq<0>{}, eq<1>{}, eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{}, eq<1>{}, eq<1>{})),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(eq<0>{}, eq<1>{}, eq<2>{})
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unique(list(eq<0>{}, eq<0>{},
                                eq<1>{},
                                eq<2>{}, eq<2>{}, eq<2>{},
                                eq<3>{}, eq<3>{}, eq<3>{},
                                eq<0>{})),
                    list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<0>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // span
            //////////////////////////////////////////////////////////////////
            {
                auto z = eq<999>{};
                auto prod = minimal_product;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(), equal.to(z)),
                    prod(list(), list())
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(eq<0>{}), equal.to(z)),
                    prod(list(), list(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z), equal.to(z)),
                    prod(list(z), list())
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(eq<0>{}, z), equal.to(z)),
                    prod(list(), list(eq<0>{}, z))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z, eq<0>{}), equal.to(z)),
                    prod(list(z), list(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(eq<0>{}, eq<1>{}), equal.to(z)),
                    prod(list(), list(eq<0>{}, eq<1>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(eq<0>{}, eq<1>{}, eq<2>{}), equal.to(z)),
                    prod(list(), list(eq<0>{}, eq<1>{}, eq<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z, eq<1>{}, eq<2>{}), equal.to(z)),
                    prod(list(z), list(eq<1>{}, eq<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(eq<0>{}, z, eq<2>{}), equal.to(z)),
                    prod(list(), list(eq<0>{}, z, eq<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z, z, eq<2>{}), equal.to(z)),
                    prod(list(z, z), list(eq<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span(list(z, z, z), equal.to(z)),
                    prod(list(z, z, z), list())
                ));

                // span.by
                BOOST_HANA_CONSTANT_CHECK(equal(
                    span.by(equal.to(z), list(eq<0>{}, eq<1>{}, eq<2>{})),
                    span(list(eq<0>{}, eq<1>{}, eq<2>{}), equal.to(z))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    span.by(equal.to(z))(list(eq<0>{}, eq<1>{}, eq<2>{})),
                    span(list(eq<0>{}, eq<1>{}, eq<2>{}), equal.to(z))
                ));
            }

#endif
#if !defined(BOOST_HANA_TEST_SEQUENCE_PART) || BOOST_HANA_TEST_SEQUENCE_PART == 2
            //////////////////////////////////////////////////////////////////
            // take_while
            //////////////////////////////////////////////////////////////////
            {
                auto z = eq<999>{};

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(), not_equal.to(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(eq<1>{}), not_equal.to(z)),
                    list(eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(z), not_equal.to(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(eq<1>{}, eq<2>{}), not_equal.to(z)),
                    list(eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(eq<1>{}, z), not_equal.to(z)),
                    list(eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(z, eq<2>{}), not_equal.to(z)),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(eq<1>{}, eq<2>{}, eq<3>{}), not_equal.to(z)),
                    list(eq<1>{}, eq<2>{}, eq<3>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(eq<1>{}, eq<2>{}, z), not_equal.to(z)),
                    list(eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(eq<1>{}, z, eq<3>{}), not_equal.to(z)),
                    list(eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    take_while(list(z, eq<2>{}, eq<3>{}), not_equal.to(z)),
                    list()
                ));
            }

            //////////////////////////////////////////////////////////////////
            // permutations
            //////////////////////////////////////////////////////////////////
            {
                auto permute = [=](auto xs) {
                    return hana::capture(xs)([=](auto xs, auto ...expected_) {
                        auto actual = hana::permutations(xs);
                        auto expected = list(expected_...);
                        BOOST_HANA_CONSTANT_CHECK(hana::and_(
                            hana::equal(hana::length(expected), hana::length(actual)),
                            hana::all_of(actual, hana::in ^ expected)
                        ));
                    });
                };

                BOOST_HANA_CONSTANT_CHECK(equal(
                    permutations(list()),
                    list(list())
                ));

                permute(list(eq<0>{}))(list(eq<0>{}));

                permute(list(eq<0>{}, eq<1>{}))(
                    list(eq<0>{}, eq<1>{}),
                    list(eq<1>{}, eq<0>{})
                );
                permute(list(eq<0>{}, eq<1>{}, eq<2>{}))(
                    list(eq<0>{}, eq<1>{}, eq<2>{}),
                    list(eq<0>{}, eq<2>{}, eq<1>{}),
                    list(eq<1>{}, eq<0>{}, eq<2>{}),
                    list(eq<1>{}, eq<2>{}, eq<0>{}),
                    list(eq<2>{}, eq<0>{}, eq<1>{}),
                    list(eq<2>{}, eq<1>{}, eq<0>{})
                );

                permute(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}))(
                    list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}),
                    list(eq<0>{}, eq<1>{}, eq<3>{}, eq<2>{}),
                    list(eq<0>{}, eq<2>{}, eq<1>{}, eq<3>{}),
                    list(eq<0>{}, eq<2>{}, eq<3>{}, eq<1>{}),
                    list(eq<0>{}, eq<3>{}, eq<1>{}, eq<2>{}),
                    list(eq<0>{}, eq<3>{}, eq<2>{}, eq<1>{}),
                    list(eq<1>{}, eq<0>{}, eq<2>{}, eq<3>{}),
                    list(eq<1>{}, eq<0>{}, eq<3>{}, eq<2>{}),
                    list(eq<1>{}, eq<2>{}, eq<0>{}, eq<3>{}),
                    list(eq<1>{}, eq<2>{}, eq<3>{}, eq<0>{}),
                    list(eq<1>{}, eq<3>{}, eq<0>{}, eq<2>{}),
                    list(eq<1>{}, eq<3>{}, eq<2>{}, eq<0>{}),
                    list(eq<2>{}, eq<0>{}, eq<1>{}, eq<3>{}),
                    list(eq<2>{}, eq<0>{}, eq<3>{}, eq<1>{}),
                    list(eq<2>{}, eq<1>{}, eq<0>{}, eq<3>{}),
                    list(eq<2>{}, eq<1>{}, eq<3>{}, eq<0>{}),
                    list(eq<2>{}, eq<3>{}, eq<0>{}, eq<1>{}),
                    list(eq<2>{}, eq<3>{}, eq<1>{}, eq<0>{}),
                    list(eq<3>{}, eq<0>{}, eq<1>{}, eq<2>{}),
                    list(eq<3>{}, eq<0>{}, eq<2>{}, eq<1>{}),
                    list(eq<3>{}, eq<1>{}, eq<0>{}, eq<2>{}),
                    list(eq<3>{}, eq<1>{}, eq<2>{}, eq<0>{}),
                    list(eq<3>{}, eq<2>{}, eq<0>{}, eq<1>{}),
                    list(eq<3>{}, eq<2>{}, eq<1>{}, eq<0>{})
                );
            }

            //////////////////////////////////////////////////////////////////
            // partition
            //////////////////////////////////////////////////////////////////
            {
                auto prod = minimal_product;
                auto pred = in ^ list(eq<-1>{}, eq<-2>{}, eq<-3>{}, eq<-4>{}, eq<-5>{});

                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(), undefined{}),
                    prod(list(), list())
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(eq<0>{}), pred),
                    prod(list(),
                         list(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(eq<0>{}, eq<1>{}), pred),
                    prod(list(),
                         list(eq<0>{}, eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(eq<-1>{}), pred),
                    prod(list(eq<-1>{}),
                         list())
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(eq<-1>{}, eq<0>{}, eq<2>{}), pred),
                    prod(list(eq<-1>{}),
                         list(eq<0>{}, eq<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(eq<0>{}, eq<-3>{}, eq<2>{}, eq<-5>{}, eq<6>{}), pred),
                    prod(list(eq<-3>{}, eq<-5>{}),
                         list(eq<0>{}, eq<2>{}, eq<6>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition(list(eq<-1>{}, eq<2>{}, eq<-3>{}, eq<0>{}, eq<-3>{}, eq<4>{}), pred),
                    prod(list(eq<-1>{}, eq<-3>{}, eq<-3>{}),
                         list(eq<2>{}, eq<0>{}, eq<4>{}))
                ));

                // partition.by
                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition.by(pred, list(eq<-1>{}, eq<0>{}, eq<2>{})),
                    partition(list(eq<-1>{}, eq<0>{}, eq<2>{}), pred)
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    partition.by(pred)(list(eq<-1>{}, eq<0>{}, eq<2>{})),
                    partition(list(eq<-1>{}, eq<0>{}, eq<2>{}), pred)
                ));
            }

            //////////////////////////////////////////////////////////////////
            // scan_left (with initial state)
            //////////////////////////////////////////////////////////////////
            {
                _injection<0> f{};
                auto s = eq<999>{};

                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(), s, undefined{}),
                    list(s)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(eq<0>{}), s, f),
                    list(s, f(s, eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(eq<0>{}, eq<1>{}), s, f),
                    list(s, f(s, eq<0>{}), f(f(s, eq<0>{}), eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(eq<0>{}, eq<1>{}, eq<2>{}), s, f),
                    list(
                        s,
                        f(s, eq<0>{}),
                        f(f(s, eq<0>{}), eq<1>{}),
                        f(f(f(s, eq<0>{}), eq<1>{}), eq<2>{})
                    )
                ));
            }

            //////////////////////////////////////////////////////////////////
            // scan_left (without initial state)
            //////////////////////////////////////////////////////////////////
            {
                _injection<0> f{};

                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(), undefined{}),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(eq<0>{}), undefined{}),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(eq<0>{}, eq<1>{}), f),
                    list(eq<0>{}, f(eq<0>{}, eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(eq<0>{}, eq<1>{}, eq<2>{}), f),
                    list(
                        eq<0>{},
                        f(eq<0>{}, eq<1>{}),
                        f(f(eq<0>{}, eq<1>{}), eq<2>{})
                    )
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_left(list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}), f),
                    list(
                        eq<0>{},
                        f(eq<0>{}, eq<1>{}),
                        f(f(eq<0>{}, eq<1>{}), eq<2>{}),
                        f(f(f(eq<0>{}, eq<1>{}), eq<2>{}), eq<3>{})
                    )
                ));
            }

            //////////////////////////////////////////////////////////////////
            // scan_right (with initial state)
            //////////////////////////////////////////////////////////////////
            {
                _injection<0> f{};
                auto s = eq<999>{};

                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_right(list(), s, undefined{}),
                    list(s)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_right(list(eq<0>{}), s, f),
                    list(
                        f(eq<0>{}, s),
                        s
                    )
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_right(list(eq<0>{}, eq<1>{}), s, f),
                    list(
                        f(eq<0>{}, f(eq<1>{}, s)),
                        f(eq<1>{}, s),
                        s
                    )
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_right(list(eq<0>{}, eq<1>{}, eq<2>{}), s, f),
                    list(
                        f(eq<0>{}, f(eq<1>{}, f(eq<2>{}, s))),
                        f(eq<1>{}, f(eq<2>{}, s)),
                        f(eq<2>{}, s),
                        s
                    )
                ));
            }

            //////////////////////////////////////////////////////////////////
            // scan_right (without initial state)
            //////////////////////////////////////////////////////////////////
            {
                _injection<0> f{};

                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_right(list(), undefined{}),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_right(list(eq<0>{}), undefined{}),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_right(list(eq<0>{}, eq<1>{}), f),
                    list(
                        f(eq<0>{}, eq<1>{}),
                        eq<1>{}
                    )
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    scan_right(list(eq<0>{}, eq<1>{}, eq<2>{}), f),
                    list(
                        f(eq<0>{}, f(eq<1>{}, eq<2>{})),
                        f(eq<1>{}, eq<2>{}),
                        eq<2>{}
                    )
                ));
            }

            //////////////////////////////////////////////////////////////////
            // cartesian_product
            //////////////////////////////////////////////////////////////////
            {
                // 0 lists
                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list()),
                    list()
                ));

                // 1 list
                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list()
                    )),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<0>{})
                    )),
                    list(
                        list(eq<0>{})
                    )
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<0>{}, eq<1>{})
                    )),
                    list(
                        list(eq<0>{}),
                        list(eq<1>{})
                    )
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<0>{}, eq<1>{}, eq<2>{})
                    )),
                    list(
                        list(eq<0>{}),
                        list(eq<1>{}),
                        list(eq<2>{})
                    )
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
                    )),
                    list(
                        list(eq<0>{}),
                        list(eq<1>{}),
                        list(eq<2>{}),
                        list(eq<3>{})
                    )
                ));

                // 2 lists
                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(),
                        list()
                    )),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<00>{}),
                        list()
                    )),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<00>{}),
                        list(eq<10>{})
                    )),
                    list(
                        list(eq<00>{}, eq<10>{})
                    )
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<00>{}, eq<01>{}),
                        list(eq<10>{})
                    )),
                    list(
                        list(eq<00>{}, eq<10>{}),
                        list(eq<01>{}, eq<10>{})
                    )
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<00>{}),
                        list(eq<10>{}, eq<11>{})
                    )),
                    list(
                        list(eq<00>{}, eq<10>{}),
                        list(eq<00>{}, eq<11>{})
                    )
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<00>{}, eq<01>{}),
                        list(eq<10>{}, eq<11>{})
                    )),
                    list(
                        list(eq<00>{}, eq<10>{}),
                        list(eq<00>{}, eq<11>{}),
                        list(eq<01>{}, eq<10>{}),
                        list(eq<01>{}, eq<11>{})
                    )
                ));

                // misc
                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<00>{}),
                        list(eq<10>{}),
                        list(eq<20>{}),
                        list(eq<30>{}, eq<31>{})
                    )),
                    list(
                        list(eq<00>{}, eq<10>{}, eq<20>{}, eq<30>{}),
                        list(eq<00>{}, eq<10>{}, eq<20>{}, eq<31>{})
                    )
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    cartesian_product(list(
                        list(eq<00>{}),
                        list(eq<10>{}),
                        list(eq<20>{}, eq<21>{}),
                        list(eq<30>{}, eq<31>{})
                    )),
                    list(
                        list(eq<00>{}, eq<10>{}, eq<20>{}, eq<30>{}),
                        list(eq<00>{}, eq<10>{}, eq<20>{}, eq<31>{}),
                        list(eq<00>{}, eq<10>{}, eq<21>{}, eq<30>{}),
                        list(eq<00>{}, eq<10>{}, eq<21>{}, eq<31>{})
                    )
                ));
            }

            //////////////////////////////////////////////////////////////////
            // drop_back
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(), size_c<0>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(), size_c<1>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(), size_c<2>),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}), size_c<0>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}), size_c<1>),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}), size_c<2>),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<1>{}), size_c<0>),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<1>{}), size_c<1>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<1>{}), size_c<2>),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<0>),
                    list(eq<0>{}, eq<1>{}, eq<2>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<1>),
                    list(eq<0>{}, eq<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<2>),
                    list(eq<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<1>{}, eq<2>{}), size_c<3>),
                    list()
                ));

                // make sure drop_back(xs) == drop_back(xs, size_c<1>)
                BOOST_HANA_CHECK(equal(
                    drop_back(list()),
                    drop_back(list(), size_c<1>)
                ));

                BOOST_HANA_CHECK(equal(
                    drop_back(list(eq<0>{})),
                    drop_back(list(eq<0>{}), size_c<1>)
                ));

                BOOST_HANA_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<1>{})),
                    drop_back(list(eq<0>{}, eq<1>{}), size_c<1>)
                ));

                BOOST_HANA_CHECK(equal(
                    drop_back(list(eq<0>{}, eq<2>{})),
                    drop_back(list(eq<0>{}, eq<2>{}), size_c<1>)
                ));
            }

#endif
#if !defined(BOOST_HANA_TEST_SEQUENCE_PART) || BOOST_HANA_TEST_SEQUENCE_PART == 3

            //////////////////////////////////////////////////////////////////
            // unfold_left
            //////////////////////////////////////////////////////////////////
            {
                auto prod = minimal_product;
                _injection<0> f{};
                auto stop_at = [=](auto stop) {
                    return [=](auto x) {
                        return hana::if_(hana::equal(stop, x),
                            hana::nothing,
                            hana::just(prod(x + hana::int_c<1>, f(x)))
                        );
                    };
                };

                BOOST_HANA_CONSTANT_CHECK(equal(
                    unfold_left<S>(int_c<0>, stop_at(int_c<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unfold_left<S>(int_c<0>, stop_at(int_c<1>)),
                    list(f(int_c<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unfold_left<S>(int_c<0>, stop_at(int_c<2>)),
                    list(f(int_c<1>), f(int_c<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unfold_left<S>(int_c<0>, stop_at(int_c<3>)),
                    list(f(int_c<2>), f(int_c<1>), f(int_c<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    unfold_left<S>(int_c<0>, stop_at(int_c<4>)),
                    list(f(int_c<3>), f(int_c<2>), f(int_c<1>), f(int_c<0>))
                ));
            }

            //////////////////////////////////////////////////////////////////
            // unfold_right
            //////////////////////////////////////////////////////////////////
            {
                auto prod = minimal_product;
                _injection<0> f{};
                auto stop_at = [=](auto stop) {
                    return [=](auto x) {
                        return hana::if_(hana::equal(stop, x),
                            nothing,
                            hana::just(prod(f(x), x + hana::int_c<1>))
                        );
                    };
                };

                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    unfold_right<S>(int_c<0>, stop_at(int_c<0>)),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    unfold_right<S>(int_c<0>, stop_at(int_c<1>)),
                    list(f(int_c<0>))
                ));
                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    unfold_right<S>(int_c<0>, stop_at(int_c<2>)),
                    list(f(int_c<0>), f(int_c<1>))
                ));
                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    unfold_right<S>(int_c<0>, stop_at(int_c<3>)),
                    list(f(int_c<0>), f(int_c<1>), f(int_c<2>))
                ));
                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    unfold_right<S>(int_c<0>, stop_at(int_c<4>)),
                    list(f(int_c<0>), f(int_c<1>), f(int_c<2>), f(int_c<3>))
                ));
            }

            //////////////////////////////////////////////////////////////////
            // Make sure unfolds can be reversed under certain conditions.
            //////////////////////////////////////////////////////////////////
            {
                auto prod = minimal_product;
                auto z = eq<999>{};
                auto f = prod;
                auto g = [=](auto k) {
                    return if_(equal(k, z),
                        nothing,
                        just(k)
                    );
                };

                // Make sure the special conditions are met
                BOOST_HANA_CONSTANT_CHECK(equal(
                    g(z),
                    nothing
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    g(f(eq<0>{}, z)),
                    just(prod(eq<0>{}, z))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    g(f(z, eq<0>{})),
                    just(prod(z, eq<0>{}))
                ));

                // Make sure the reversing works
                auto lists = list(
                    list(),
                    list(eq<0>{}),
                    list(eq<0>{}, eq<1>{}),
                    list(eq<0>{}, eq<1>{}, eq<2>{})
                );
                for_each(lists, capture(z, f, g)([](auto z, auto f, auto g, auto xs) {
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfold_left<S>(fold_left(xs, z, f), g),
                        xs
                    ));
                    BOOST_HANA_CONSTANT_CHECK(equal(
                        unfold_right<S>(fold_right(xs, z, f), g),
                        xs
                    ));
                }));
            }

            //////////////////////////////////////////////////////////////////
            // sort (without a custom predicate)
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<0>{})),
                    list(ord<0>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<0>{}, ord<1>{})),
                    list(ord<0>{}, ord<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<1>{}, ord<0>{})),
                    list(ord<0>{}, ord<1>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<1>{}, ord<0>{}, ord<4>{}, ord<2>{})),
                    list(ord<0>{}, ord<1>{}, ord<2>{}, ord<4>{})
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(ord<1>{}, ord<0>{}, ord<-4>{}, ord<2>{})),
                    list(ord<-4>{}, ord<0>{}, ord<1>{}, ord<2>{})
                ));
            }

            //////////////////////////////////////////////////////////////////
            // sort (with a custom predicate)
            //////////////////////////////////////////////////////////////////
            {
                auto pred = [](auto x, auto y) {
                    return less(x.unwrap, y.unwrap);
                };
                auto a = [](auto z) { return test::tag(eq<999>{}, z); };
                auto b = [](auto z) { return test::tag(eq<888>{}, z); };

                auto check = [=](auto ...sorted) {
                    auto perms = transform(
                        permutations(list(a(sorted)...)),
                        sort.by(pred)
                    );
                    BOOST_HANA_CONSTANT_CHECK(all_of(perms, [=](auto xs) {
                        return equal(xs, list(a(sorted)...));
                    }));
                };

                check();
                check(ord<1>{});
                check(ord<1>{}, ord<2>{});
                check(ord<1>{}, ord<2>{}, ord<3>{});

                // check stability
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(a(ord<1>{}), b(ord<1>{})), pred),
                    list(a(ord<1>{}), b(ord<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(b(ord<1>{}), a(ord<1>{})), pred),
                    list(b(ord<1>{}), a(ord<1>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(a(ord<1>{}), b(ord<1>{}), a(ord<2>{}), b(ord<2>{})), pred),
                    list(a(ord<1>{}), b(ord<1>{}), a(ord<2>{}), b(ord<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(a(ord<1>{}), a(ord<2>{}), b(ord<1>{}), b(ord<2>{})), pred),
                    list(a(ord<1>{}), b(ord<1>{}), a(ord<2>{}), b(ord<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(b(ord<1>{}), a(ord<1>{}), a(ord<2>{}), b(ord<2>{})), pred),
                    list(b(ord<1>{}), a(ord<1>{}), a(ord<2>{}), b(ord<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(a(ord<2>{}), b(ord<1>{}), b(ord<2>{}), a(ord<1>{})), pred),
                    list(b(ord<1>{}), a(ord<1>{}), a(ord<2>{}), b(ord<2>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    sort(list(a(ord<1>{}), a(ord<3>{}), b(ord<1>{}), a(ord<2>{}), b(ord<3>{})), pred),
                    list(a(ord<1>{}), b(ord<1>{}), a(ord<2>{}), a(ord<3>{}), b(ord<3>{}))
                ));
            }

            //////////////////////////////////////////////////////////////////
            // group (without a custom predicate)
            //////////////////////////////////////////////////////////////////
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<0>{})),
                    list(list(eq<0>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<0>{}, eq<0>{})),
                    list(list(eq<0>{}, eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<0>{}, eq<1>{})),
                    list(list(eq<0>{}), list(eq<1>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<0>{}, eq<0>{}, eq<0>{})),
                    list(list(eq<0>{}, eq<0>{}, eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<0>{}, eq<0>{}, eq<1>{})),
                    list(list(eq<0>{}, eq<0>{}), list(eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<0>{}, eq<1>{}, eq<0>{})),
                    list(list(eq<0>{}),
                         list(eq<1>{}),
                         list(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<1>{}, eq<0>{}, eq<0>{})),
                    list(list(eq<1>{}),
                         list(eq<0>{}, eq<0>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<0>{}, eq<0>{}, eq<1>{}, eq<1>{})),
                    list(list(eq<0>{}, eq<0>{}),
                         list(eq<1>{}, eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(eq<0>{}, eq<0>{}, eq<1>{}, eq<1>{}, eq<2>{}, eq<2>{}, eq<2>{})),
                    list(list(eq<0>{}, eq<0>{}),
                         list(eq<1>{}, eq<1>{}),
                         list(eq<2>{}, eq<2>{}, eq<2>{}))
                ));
            }

            //////////////////////////////////////////////////////////////////
            // group (with a custom predicate)
            //////////////////////////////////////////////////////////////////
            {
                auto a = [](auto z) { return test::tag(eq<999>{}, z); };
                auto b = [](auto z) { return test::tag(eq<888>{}, z); };

                BOOST_HANA_CONSTEXPR_LAMBDA auto pred = [](auto x, auto y) {
                    return equal(x.unwrap, y.unwrap);
                };

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(), undefined{}),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(a(eq<0>{})), pred),
                    list(list(a(eq<0>{})))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(a(eq<0>{}), b(eq<0>{})), pred),
                    list(list(a(eq<0>{}), b(eq<0>{})))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(a(eq<0>{}), b(eq<0>{}), a(eq<1>{})), pred),
                    list(list(a(eq<0>{}), b(eq<0>{})), list(a(eq<1>{})))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(a(eq<0>{}), b(eq<0>{}), a(eq<1>{}), b(eq<1>{})), pred),
                    list(list(a(eq<0>{}), b(eq<0>{})), list(a(eq<1>{}), b(eq<1>{})))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    group(list(a(eq<0>{}), b(eq<0>{}), a(eq<1>{}), b(eq<1>{}), b(eq<0>{})), pred),
                    list(list(a(eq<0>{}), b(eq<0>{})), list(a(eq<1>{}), b(eq<1>{})), list(b(eq<0>{})))
                ));
            }

            //////////////////////////////////////////////////////////////////
            // zip_shortest
            //////////////////////////////////////////////////////////////////
            {
                auto t = list; // tests are unreadable otherwise. mnemonic: tuple

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_shortest(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_shortest(list(eq<0>{})),
                    list(t(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_shortest(list(eq<0>{}, eq<1>{})),
                    list(t(eq<0>{}), t(eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_shortest(list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(t(eq<0>{}), t(eq<1>{}), t(eq<2>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_shortest(list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_shortest(list(eq<0>{}), list(eq<1>{})),
                    list(t(eq<0>{}, eq<1>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_shortest(list(eq<0>{}), list(eq<1>{}), list(eq<2>{})),
                    list(t(eq<0>{}, eq<1>{}, eq<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_shortest(list(eq<0>{}, eq<3>{}),
                                 list(eq<1>{}, eq<4>{}),
                                 list(eq<2>{}, eq<5>{}, eq<8>{})),
                    list(t(eq<0>{}, eq<1>{}, eq<2>{}),
                         t(eq<3>{}, eq<4>{}, eq<5>{}))
                ));
            }

            //////////////////////////////////////////////////////////////////
            // zip
            //////////////////////////////////////////////////////////////////
            {
                auto t = list; // tests are unreadable otherwise. mnemonic: tuple

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(eq<0>{})),
                    list(t(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(eq<0>{}, eq<1>{})),
                    list(t(eq<0>{}), t(eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(t(eq<0>{}), t(eq<1>{}), t(eq<2>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(eq<0>{}), list(eq<1>{})),
                    list(t(eq<0>{}, eq<1>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(eq<0>{}), list(eq<1>{}), list(eq<2>{})),
                    list(t(eq<0>{}, eq<1>{}, eq<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(list(eq<0>{}, eq<3>{}),
                        list(eq<1>{}, eq<4>{}),
                        list(eq<2>{}, eq<5>{})),
                    list(t(eq<0>{}, eq<1>{}, eq<2>{}),
                         t(eq<3>{}, eq<4>{}, eq<5>{}))
                ));
            }

            //////////////////////////////////////////////////////////////////
            // zip_with
            //////////////////////////////////////////////////////////////////
            {
                _injection<0> f{};

                // zip 1
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined{}, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(eq<0>{})),
                    list(f(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(eq<0>{}, eq<1>{})),
                    list(f(eq<0>{}), f(eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(f(eq<0>{}), f(eq<1>{}), f(eq<2>{}))
                ));

                // zip 2
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined{}, list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(eq<1>{}), list(eq<-1>{})),
                    list(f(eq<1>{}, eq<-1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(eq<1>{}, eq<2>{}), list(eq<-1>{}, eq<-2>{})),
                    list(f(eq<1>{}, eq<-1>{}), f(eq<2>{}, eq<-2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(eq<1>{}, eq<2>{}, eq<3>{}),
                                list(eq<-1>{}, eq<-2>{}, eq<-3>{})),
                    list(f(eq<1>{}, eq<-1>{}),
                         f(eq<2>{}, eq<-2>{}),
                         f(eq<3>{}, eq<-3>{}))
                ));

                // zip 3
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(undefined{}, list(), list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(eq<0>{}), list(eq<1>{}), list(eq<2>{})),
                    list(f(eq<0>{}, eq<1>{}, eq<2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f, list(eq<0>{}, eq<1>{}),
                                list(eq<2>{}, eq<3>{}),
                                list(eq<4>{}, eq<5>{})),
                    list(f(eq<0>{}, eq<2>{}, eq<4>{}),
                         f(eq<1>{}, eq<3>{}, eq<5>{}))
                ));

                // zip 4
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f,
                        list(eq<11>{}, eq<12>{}, eq<13>{}),
                        list(eq<21>{}, eq<22>{}, eq<23>{}),
                        list(eq<31>{}, eq<32>{}, eq<33>{}),
                        list(eq<41>{}, eq<42>{}, eq<43>{})
                    ),
                    list(
                        f(eq<11>{}, eq<21>{}, eq<31>{}, eq<41>{}),
                        f(eq<12>{}, eq<22>{}, eq<32>{}, eq<42>{}),
                        f(eq<13>{}, eq<23>{}, eq<33>{}, eq<43>{})
                    )
                ));

                // zip 5
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip_with(f,
                        list(eq<11>{}, eq<12>{}, eq<13>{}, eq<14>{}),
                        list(eq<21>{}, eq<22>{}, eq<23>{}, eq<24>{}),
                        list(eq<31>{}, eq<32>{}, eq<33>{}, eq<34>{}),
                        list(eq<41>{}, eq<42>{}, eq<43>{}, eq<44>{}),
                        list(eq<51>{}, eq<52>{}, eq<53>{}, eq<54>{})
                    ),
                    list(
                        f(eq<11>{}, eq<21>{}, eq<31>{}, eq<41>{}, eq<51>{}),
                        f(eq<12>{}, eq<22>{}, eq<32>{}, eq<42>{}, eq<52>{}),
                        f(eq<13>{}, eq<23>{}, eq<33>{}, eq<43>{}, eq<53>{}),
                        f(eq<14>{}, eq<24>{}, eq<34>{}, eq<44>{}, eq<54>{})
                    )
                ));
            }

            //////////////////////////////////////////////////////////////////
            // zip_shortest_with
            //////////////////////////////////////////////////////////////////
            {
                _injection<0> f{};
                auto zip = hana::zip_shortest_with;

                // zip 1
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<0>{})),
                    list(f(eq<0>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<0>{}, eq<1>{})),
                    list(f(eq<0>{}), f(eq<1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<0>{}, eq<1>{}, eq<2>{})),
                    list(f(eq<0>{}), f(eq<1>{}), f(eq<2>{}))
                ));

                // zip 2
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(undefined{}), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(), list(undefined{})),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<1>{}), list(eq<-1>{})),
                    list(f(eq<1>{}, eq<-1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<1>{}, eq<2>{}), list(eq<-1>{})),
                    list(f(eq<1>{}, eq<-1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<1>{}), list(eq<-1>{}, eq<-2>{})),
                    list(f(eq<1>{}, eq<-1>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<1>{}, eq<2>{}), list(eq<-1>{}, eq<-2>{})),
                    list(f(eq<1>{}, eq<-1>{}), f(eq<2>{}, eq<-2>{}))
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}),
                           list(eq<-1>{}, eq<-2>{}, eq<-3>{})),
                    list(f(eq<1>{}, eq<-1>{}),
                         f(eq<2>{}, eq<-2>{}),
                         f(eq<3>{}, eq<-3>{}))
                ));

                // zip 3
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(), list(), list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(undefined{}), list(), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(), list(undefined{}), list()),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(), list(), list(undefined{})),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(), list(undefined{}), list(undefined{})),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(undefined{}), list(), list(undefined{})),
                    list()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(undefined{}, list(undefined{}), list(undefined{}), list()),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f, list(eq<0>{}), list(eq<1>{}), list(eq<2>{})),
                    list(f(eq<0>{}, eq<1>{}, eq<2>{}))
                ));

                // zip 4
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f,
                        list(eq<11>{}, eq<12>{}, eq<13>{}, eq<14>{}),
                        list(eq<21>{}, eq<22>{}, eq<23>{}),
                        list(eq<31>{}, eq<32>{}, eq<33>{}, eq<34>{}),
                        list(eq<41>{}, eq<42>{}, eq<43>{}, eq<44>{}, eq<45>{})
                    ),
                    list(
                        f(eq<11>{}, eq<21>{}, eq<31>{}, eq<41>{}),
                        f(eq<12>{}, eq<22>{}, eq<32>{}, eq<42>{}),
                        f(eq<13>{}, eq<23>{}, eq<33>{}, eq<43>{})
                    )
                ));

                // zip 5
                BOOST_HANA_CONSTANT_CHECK(equal(
                    zip(f,
                        list(eq<11>{}, eq<12>{}, eq<13>{}, eq<14>{}),
                        list(eq<21>{}, eq<22>{}, eq<23>{}, eq<24>{}, eq<25>{}),
                        list(eq<31>{}, eq<32>{}, eq<33>{}, eq<34>{}),
                        list(eq<41>{}, eq<42>{}, eq<43>{}, eq<44>{}, eq<45>{}, eq<46>{}),
                        list(eq<51>{}, eq<52>{}, eq<53>{}, eq<54>{}, eq<55>{})
                    ),
                    list(
                        f(eq<11>{}, eq<21>{}, eq<31>{}, eq<41>{}, eq<51>{}),
                        f(eq<12>{}, eq<22>{}, eq<32>{}, eq<42>{}, eq<52>{}),
                        f(eq<13>{}, eq<23>{}, eq<33>{}, eq<43>{}, eq<53>{}),
                        f(eq<14>{}, eq<24>{}, eq<34>{}, eq<44>{}, eq<54>{})
                    )
                ));
            }
#endif
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_SEQUENCE_HPP
