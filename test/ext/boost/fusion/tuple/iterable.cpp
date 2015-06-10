/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#define FUSION_MAX_VECTOR_SIZE 20
#include <boost/hana/ext/boost/fusion/tuple.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/foldable.hpp>
#include <laws/iterable.hpp>

#include <boost/fusion/tuple.hpp>
using namespace boost::hana;
namespace fusion = boost::fusion;


template <int i>
using eq = test::ct_eq<i>;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_tuples = make<Tuple>(
          fusion::make_tuple()
        , fusion::make_tuple(eq<0>{})
        , fusion::make_tuple(eq<0>{}, eq<1>{})
        , fusion::make_tuple(eq<0>{}, eq<1>{}, eq<2>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Foldable, Iterable
    //////////////////////////////////////////////////////////////////////////
    test::TestFoldable<ext::boost::fusion::Tuple>{eq_tuples};
    test::TestIterable<ext::boost::fusion::Tuple>{eq_tuples};
}
