/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/deque.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/searchable.hpp>

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

    auto eq_deque_keys = make<Tuple>(
        eq<1>{}, eq<5>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestSearchable<ext::boost::fusion::Deque>{eq_deques, eq_deque_keys};

        auto bools = make<Tuple>(
              fusion::make_deque(true_)
            , fusion::make_deque(false_)
            , fusion::make_deque(true_, true_)
            , fusion::make_deque(true_, false_)
            , fusion::make_deque(false_, true_)
            , fusion::make_deque(false_, false_)
        );
        test::TestSearchable<ext::boost::fusion::Deque>{bools, make<Tuple>(true_, false_)};
    }
}
