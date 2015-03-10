/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>
#include <test/identity.hpp>
using namespace boost::hana;


int main() {
    using test::ct_eq;
    struct undefined { };
    test::_injection<0> f{};

    auto eqs = make<Tuple>(
        right(ct_eq<0>{}), left(ct_eq<0>{}),
        right(ct_eq<1>{}), left(ct_eq<1>{}),
        right(ct_eq<2>{}), left(ct_eq<2>{}),
        right(ct_eq<3>{}), left(ct_eq<3>{})
    );

    auto nested_eqs = make<Tuple>(
        right(right(ct_eq<2>{})), left(right(ct_eq<2>{})),
        right(right(ct_eq<3>{})), left(right(ct_eq<3>{})),

        right(left(ct_eq<0>{})), left(left(ct_eq<0>{})),
        right(left(ct_eq<1>{})), left(left(ct_eq<1>{}))
    );

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    {
        // transform
        {
            test::_injection<0> f{};
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(left(ct_eq<0>{}), undefined{}),
                left(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(right(ct_eq<0>{}), f),
                right(f(ct_eq<0>{}))
            ));
        }

        // laws
        test::TestFunctor<Either>{eqs};
    }

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    {
        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(left(ct_eq<0>{}), left(undefined{})),
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(left(ct_eq<0>{}), right(undefined{})),
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(right(undefined{}), left(ct_eq<0>{})),
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(right(f), right(ct_eq<0>{})),
                right(f(ct_eq<0>{}))
            ));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<Either>(ct_eq<0>{}),
                right(ct_eq<0>{})
            ));
        }

        // laws
        test::TestApplicative<Either>{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    {
        // flatten
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(left(left(ct_eq<0>{}))),
                left(left(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(left(right(ct_eq<0>{}))),
                left(right(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(right(left(ct_eq<0>{}))),
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(right(right(ct_eq<0>{}))),
                right(ct_eq<0>{})
            ));
        }

        // laws
        test::TestMonad<Either>{eqs, nested_eqs};
    }
}
