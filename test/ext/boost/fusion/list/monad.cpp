/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/list.hpp>

#include <boost/hana/tuple.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>

#include <boost/fusion/container/generation/make_list.hpp>
#include <boost/fusion/container/list.hpp>
using namespace boost::hana;
namespace fusion = boost::fusion;


template <int i>
using eq = test::ct_eq<i>;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_lists = make<Tuple>(
          fusion::make_list()
        , fusion::make_list(eq<0>{})
        , fusion::make_list(eq<0>{}, eq<1>{})
        , fusion::make_list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
    );

    auto eq_lists_lists = make<Tuple>(
          fusion::make_list()
        , fusion::make_list(
            fusion::make_list(eq<0>{}))
        , fusion::make_list(
            fusion::make_list(eq<0>{}),
            fusion::make_list(eq<1>{}, eq<2>{}))
        , fusion::make_list(
            fusion::make_list(eq<0>{}),
            fusion::make_list(eq<1>{}, eq<2>{}),
            fusion::make_list(eq<3>{}, eq<4>{}))
    );

    //////////////////////////////////////////////////////////////////////////
    // Functor up to Monad
    //////////////////////////////////////////////////////////////////////////
    test::TestFunctor<ext::boost::fusion::List>{eq_lists};
    test::TestApplicative<ext::boost::fusion::List>{};
    test::TestMonad<ext::boost::fusion::List>{eq_lists, eq_lists_lists};
}
