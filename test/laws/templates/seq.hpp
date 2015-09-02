/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <test/seq.hpp>

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
#include <laws/sequence.hpp>
using namespace boost::hana;


int main() {
    using boost::hana::size_t;
    using test::ct_eq;
    using test::ct_ord;

    auto eqs = make<Tuple>(
          test::seq()
        , test::seq(ct_eq<0>{})
        , test::seq(ct_eq<0>{}, ct_eq<1>{})
        , test::seq(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        , test::seq(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
    );
    (void)eqs;

    auto nested_eqs = make<Tuple>(
          test::seq()
        , test::seq(
            test::seq(ct_eq<0>{}))
        , test::seq(
            test::seq(ct_eq<0>{}),
            test::seq(ct_eq<1>{}, ct_eq<2>{}))
        , test::seq(
            test::seq(ct_eq<0>{}),
            test::seq(ct_eq<1>{}, ct_eq<2>{}),
            test::seq(ct_eq<3>{}, ct_eq<4>{}))
    );
    (void)nested_eqs;

    auto eq_keys = make<Tuple>(ct_eq<0>{}, ct_eq<3>{}, ct_eq<10>{});
    (void)eq_keys;

    auto predicates = make<Tuple>(
        equal.to(ct_eq<0>{}), equal.to(ct_eq<3>{}), equal.to(ct_eq<10>{}),
        always(true_c), always(false_c)
    );
    (void)predicates;

    auto ords = make<Tuple>(
          test::seq()
        , test::seq(ct_ord<0>{})
        , test::seq(ct_ord<0>{}, ct_ord<1>{})
        , test::seq(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{})
        , test::seq(ct_ord<0>{}, ct_ord<1>{}, ct_ord<2>{}, ct_ord<3>{})
    );
    (void)ords;

    //////////////////////////////////////////////////////////////////////////
    // Comparable, Orderable
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_ORDERABLE
    test::TestComparable<test::Seq>{eqs};
    test::TestOrderable<test::Seq>{ords};
#endif

#ifdef BOOST_HANA_TEST_ITERABLE
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    test::TestFoldable<test::Seq>{eqs};

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestIterable<test::Seq>{eqs};
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_SEARCHABLE
    {
        test::TestSearchable<test::Seq>{eqs, eq_keys};

        auto bools = make<Tuple>(
              test::seq(true_c)
            , test::seq(false_c)
            , test::seq(true_c, true_c)
            , test::seq(true_c, false_c)
            , test::seq(false_c, true_c)
            , test::seq(false_c, false_c)
        );
        test::TestSearchable<test::Seq>{bools, make<Tuple>(true_c, false_c)};
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Functor, Applicative, Monad
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_MONAD
    test::TestFunctor<test::Seq>{eqs, eq_keys};
    test::TestApplicative<test::Seq>{eqs};
    test::TestMonad<test::Seq>{eqs, nested_eqs};
#endif

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_MONAD_PLUS
    test::TestMonadPlus<test::Seq>{eqs, predicates, eq_keys};
#endif

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_SEQUENCE
    test::TestSequence<test::Seq>{};
#endif
}
