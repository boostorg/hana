/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/tuple.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>

#include <boost/tuple/tuple.hpp>
using namespace boost::hana;


template <int i>
using eq = test::ct_eq<i>;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_tuples = make<Tuple>(
          ::boost::make_tuple()
        , ::boost::make_tuple(eq<0>{})
        , ::boost::make_tuple(eq<0>{}, eq<1>{})
        , ::boost::make_tuple(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
    );

    auto eq_values = make<Tuple>(eq<0>{}, eq<2>{});

    auto eq_tuples_tuples = make<Tuple>(
          ::boost::make_tuple()
        , ::boost::make_tuple(
            ::boost::make_tuple(eq<0>{}))
        , ::boost::make_tuple(
            ::boost::make_tuple(eq<0>{}),
            ::boost::make_tuple(eq<1>{}, eq<2>{}))
        , ::boost::make_tuple(
            ::boost::make_tuple(eq<0>{}),
            ::boost::make_tuple(eq<1>{}, eq<2>{}),
            ::boost::make_tuple(eq<3>{}, eq<4>{}))
    );

    //////////////////////////////////////////////////////////////////////////
    // Functor up to Monad
    //////////////////////////////////////////////////////////////////////////
    test::TestFunctor<ext::boost::Tuple>{eq_tuples, eq_values};
    test::TestApplicative<ext::boost::Tuple>{};
    test::TestMonad<ext::boost::Tuple>{eq_tuples, eq_tuples_tuples};
}
