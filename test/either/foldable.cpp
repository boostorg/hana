/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/foldable.hpp>
#include <laws/traversable.hpp>
#include <test/identity.hpp>
using namespace boost::hana;


int main() {
    using test::ct_eq;

    auto eqs = make<Tuple>(
        right(ct_eq<0>{}), left(ct_eq<0>{}),
        right(ct_eq<1>{}), left(ct_eq<1>{}),
        right(ct_eq<2>{}), left(ct_eq<2>{}),
        right(ct_eq<3>{}), left(ct_eq<3>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        // unpack
        {
            test::_injection<0> f{};

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(left(ct_eq<0>{}), f),
                f()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(right(ct_eq<0>{}), f),
                f(ct_eq<0>{})
            ));
        }

        // laws
        test::TestFoldable<Either>{eqs};
    }

    //////////////////////////////////////////////////////////////////////////
    // Traversable
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};
        auto f_ = compose(lift<test::Identity>, f);

        BOOST_HANA_CONSTANT_CHECK(equal(
            traverse<test::Identity>(left(ct_eq<0>{}), f_),
            test::identity(left(ct_eq<0>{}))
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            traverse<test::Identity>(right(ct_eq<0>{}), f_),
            test::identity(right(f(ct_eq<0>{})))
        ));

        // laws
        test::TestTraversable<Either>{};
    }
}
