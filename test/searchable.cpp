/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/tuple.hpp>

#include <test/seq.hpp>

#include <laws/base.hpp>
#include <laws/searchable.hpp>
using namespace boost::hana;


using test::ct_eq;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Laws with a minimal Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        auto eqs = make<Tuple>(
              test::seq()
            , test::seq(ct_eq<0>{})
            , test::seq(ct_eq<0>{}, ct_eq<1>{})
            , test::seq(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
            , test::seq(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
        );

        auto eq_keys = make<Tuple>(ct_eq<0>{}, ct_eq<3>{}, ct_eq<10>{});

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
}
