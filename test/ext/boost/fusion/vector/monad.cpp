/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#define FUSION_MAX_VECTOR_SIZE 20
#include <boost/hana/ext/boost/fusion/vector.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>

#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/container/vector.hpp>
using namespace boost::hana;
namespace fusion = boost::fusion;


template <int i>
using eq = test::ct_eq<i>;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_vectors = make<Tuple>(
          fusion::make_vector()
        , fusion::make_vector(eq<0>{})
        , fusion::make_vector(eq<0>{}, eq<1>{})
        , fusion::make_vector(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
    );

    auto eq_values = make<Tuple>(eq<0>{}, eq<2>{});

    auto eq_vectors_vectors = make<Tuple>(
          fusion::make_vector()
        , fusion::make_vector(
            fusion::make_vector(eq<0>{}))
        , fusion::make_vector(
            fusion::make_vector(eq<0>{}),
            fusion::make_vector(eq<1>{}, eq<2>{}))
        , fusion::make_vector(
            fusion::make_vector(eq<0>{}),
            fusion::make_vector(eq<1>{}, eq<2>{}),
            fusion::make_vector(eq<3>{}, eq<4>{}))
    );

    //////////////////////////////////////////////////////////////////////////
    // Functor up to Monad
    //////////////////////////////////////////////////////////////////////////
    test::TestFunctor<ext::boost::fusion::Vector>{eq_vectors, eq_values};
    test::TestApplicative<ext::boost::fusion::Vector>{eq_vectors};
    test::TestMonad<ext::boost::fusion::Vector>{eq_vectors, eq_vectors_vectors};
}
