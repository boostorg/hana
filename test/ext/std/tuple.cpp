/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/functor.hpp>
#include <laws/iterable.hpp>
#include <laws/monad.hpp>
#include <laws/monad_plus.hpp>
#include <laws/orderable.hpp>
#include <laws/searchable.hpp>

#define BOOST_HANA_TEST_SEQUENCE_KNOWN_PARTS 3
#if BOOST_HANA_TEST_PART == 6
#   define BOOST_HANA_TEST_SEQUENCE_PART 1
#elif BOOST_HANA_TEST_PART == 7
#   define BOOST_HANA_TEST_SEQUENCE_PART 2
#elif BOOST_HANA_TEST_PART == 8
#   define BOOST_HANA_TEST_SEQUENCE_PART 3
#endif
#include <laws/sequence.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
using eq = test::ct_eq<i>;

using test::ct_ord;

int main() {
    auto big_eq_tuples = make<tuple_tag>(
          std::make_tuple()
        , std::make_tuple(eq<0>{})
        , std::make_tuple(eq<0>{}, eq<1>{})
        , std::make_tuple(eq<0>{}, eq<1>{}, eq<2>{})
        , std::make_tuple(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
    );
    (void)big_eq_tuples;

    auto small_eq_tuples = make<tuple_tag>(
          std::make_tuple()
        , std::make_tuple(eq<0>{})
        , std::make_tuple(eq<0>{}, eq<1>{})
        , std::make_tuple(eq<0>{}, eq<1>{}, eq<2>{})
    );
    (void)small_eq_tuples;

    auto ord_tuples = make<tuple_tag>(
          std::make_tuple()
        , std::make_tuple(ct_ord<0>{})
        , std::make_tuple(ct_ord<0>{}, ct_ord<1>{})
        , std::make_tuple(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{})
        , std::make_tuple(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}, ct_ord<3>{}, ct_ord<4>{})
    );
    (void)ord_tuples;

    auto eq_values = make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{});
    (void)eq_values;

    auto predicates = make<tuple_tag>(
        equal.to(eq<0>{}), equal.to(eq<1>{}), equal.to(eq<2>{}),
        always(false_c), always(true_c)
    );
    (void)predicates;

    auto nested_tuples = make<tuple_tag>(
          std::make_tuple()
        , std::make_tuple(
            std::make_tuple(eq<0>{}))
        , std::make_tuple(
            std::make_tuple(eq<0>{}),
            std::make_tuple(eq<1>{}, eq<2>{}))
        , std::make_tuple(
            std::make_tuple(eq<0>{}),
            std::make_tuple(eq<1>{}, eq<2>{}),
            std::make_tuple(eq<3>{}, eq<4>{}))
    );
    (void)nested_tuples;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        auto eq_tuple_keys = make<tuple_tag>(eq<3>{}, eq<5>{}, eq<7>{});
        test::TestSearchable<ext::std::tuple_tag>{small_eq_tuples, eq_tuple_keys};

        auto bools = make<tuple_tag>(
              std::make_tuple(true_c)
            , std::make_tuple(false_c)
            , std::make_tuple(true_c, true_c)
            , std::make_tuple(true_c, false_c)
            , std::make_tuple(false_c, true_c)
            , std::make_tuple(false_c, false_c)
        );
        test::TestSearchable<ext::std::tuple_tag>{bools, make<tuple_tag>(true_c, false_c)};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Comparable and Orderable
    //////////////////////////////////////////////////////////////////////////
    test::TestComparable<ext::std::tuple_tag>{big_eq_tuples};
    test::TestOrderable<ext::std::tuple_tag>{ord_tuples};

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    test::TestMonadPlus<ext::std::tuple_tag>{small_eq_tuples, predicates, eq_values};

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Foldable, Iterable
    //////////////////////////////////////////////////////////////////////////
    test::TestFoldable<ext::std::tuple_tag>{big_eq_tuples};
    test::TestIterable<ext::std::tuple_tag>{big_eq_tuples};

#elif BOOST_HANA_TEST_PART == 5
    //////////////////////////////////////////////////////////////////////////
    // Functor up to Monad
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestFunctor<ext::std::tuple_tag>{big_eq_tuples, eq_values};
        test::TestApplicative<ext::std::tuple_tag>{small_eq_tuples};
        test::TestMonad<ext::std::tuple_tag>{big_eq_tuples, nested_tuples};
    }

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
#elif BOOST_HANA_TEST_PART == 6 || BOOST_HANA_TEST_PART == 7 || BOOST_HANA_TEST_PART == 8
    test::TestSequence<ext::std::tuple_tag>{};
#endif
}
