/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
using namespace boost::hana;


int main() {
    using test::ct_eq;
    struct undefined { };

    auto eqs = make<Tuple>(
        right(ct_eq<0>{}), left(ct_eq<0>{}),
        right(ct_eq<2>{}), left(ct_eq<2>{}),
        right(ct_eq<3>{}), left(ct_eq<3>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {

        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                left(ct_eq<0>{}),
                left(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                left(ct_eq<0>{}),
                left(ct_eq<1>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                left(undefined{}),
                right(undefined{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                right(undefined{}),
                left(undefined{})
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                right(ct_eq<0>{}),
                right(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                right(ct_eq<0>{}),
                right(ct_eq<1>{})
            )));
        }

        // laws
        test::TestComparable<Either>{eqs};
    }
}
