/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <boost/hana/integral_constant.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>
#include <laws/monad_plus.hpp>
using namespace boost::hana;


// This test is in its own file to avoid crashing the compiler.

template <int i>
using eq = test::ct_eq<i>;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_tuples = make<Tuple>(
          make<Tuple>()
        , make<Tuple>(eq<0>{})
        , make<Tuple>(eq<0>{}, eq<1>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}, eq<5>{})
    );

    auto eq_tuples_tuples = make<Tuple>(
          make<Tuple>()
        , make<Tuple>(make<Tuple>(eq<0>{}))
        , make<Tuple>(make<Tuple>(eq<0>{}), make<Tuple>(eq<1>{}, eq<2>{}))
        , make<Tuple>(make<Tuple>(eq<0>{}),
                      make<Tuple>(eq<1>{}, eq<2>{}),
                      make<Tuple>(eq<3>{}, eq<4>{}))
    );

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestFunctor<Tuple>{eq_tuples};
    }

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestApplicative<Tuple>{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestMonad<Tuple>{eq_tuples, eq_tuples_tuples};
    }

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    {
        // prepend
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(long_<0>, tuple_c<long>),
                tuple_c<long, 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(uint<0>, tuple_c<unsigned int, 1>),
                tuple_c<unsigned int, 0, 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(llong<0>, tuple_c<long long, 1, 2>),
                tuple_c<long long, 0, 1, 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(ulong<0>, tuple_c<unsigned long, 1, 2, 3>),
                tuple_c<unsigned long, 0, 1, 2, 3>
            ));
        }

        // empty
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<Tuple>(),
                tuple_c<int>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<Tuple>(),
                tuple_c<long>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<Tuple>(),
                tuple_c<void>
            ));
        }

        test::TestMonadPlus<Tuple>{eq_tuples};
    }
}
