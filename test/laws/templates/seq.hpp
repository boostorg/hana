/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/seq.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/operators.hpp>
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
#include <laws/traversable.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    namespace operators {
        template <>
        struct of<test::Seq>
            : operators::of<Iterable>
        { };
    }
}}

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
        always(true_), always(false_)
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
    // Traversable
    //////////////////////////////////////////////////////////////////////////
    test::TestTraversable<test::Seq>{};

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        auto iterable = test::seq;

        // operators
        {
            // T&
            {
                auto lvalue = iterable(test::ct_eq<0>{});

                auto const& result = at(size_t<0>, lvalue);
                (void)result;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    lvalue[size_t<0>],
                    at(size_t<0>, lvalue)
                ));
            }

            // T const&
            {
                auto const const_lvalue = iterable(test::ct_eq<0>{});

                auto const& result = at(size_t<0>, const_lvalue);
                (void)result;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    const_lvalue[size_t<0>],
                    at(size_t<0>, const_lvalue)
                ));
            }

            // T&&
            {
                auto rvalue = [=] { return iterable(test::ct_eq<0>{}); };

                auto&& result = at(size_t<0>, rvalue());
                (void)result;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    rvalue()[size_t<0>],
                    at(size_t<0>, rvalue())
                ));
            }
        }

        // laws
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
              test::seq(true_)
            , test::seq(false_)
            , test::seq(true_, true_)
            , test::seq(true_, false_)
            , test::seq(false_, true_)
            , test::seq(false_, false_)
        );
        test::TestSearchable<test::Seq>{bools, make<Tuple>(true_, false_)};
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
