/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/deque.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/foldable.hpp>
#include <laws/iterable.hpp>
#include <laws/traversable.hpp>

#include <boost/fusion/container/generation/make_deque.hpp>
#include <boost/fusion/container/deque.hpp>
using namespace boost::hana;
namespace fusion = boost::fusion;


template <int i>
using eq = test::ct_eq<i>;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_deques = make<Tuple>(
          fusion::make_deque()
        , fusion::make_deque(eq<0>{})
        , fusion::make_deque(eq<0>{}, eq<1>{})
        , fusion::make_deque(eq<0>{}, eq<1>{}, eq<2>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Foldable, Iterable, Traversable
    //////////////////////////////////////////////////////////////////////////
    test::TestFoldable<ext::boost::fusion::Deque>{eq_deques};
    test::TestIterable<ext::boost::fusion::Deque>{eq_deques};
    test::TestTraversable<ext::boost::fusion::Deque>{};
}
