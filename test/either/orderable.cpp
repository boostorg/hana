/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/orderable.hpp>
using namespace boost::hana;


int main() {
    using test::ct_ord;
    struct undefined { };

    auto ords = make<Tuple>(
        right(ct_ord<0>{}), left(ct_ord<0>{}),
        right(ct_ord<2>{}), left(ct_ord<2>{}),
        right(ct_ord<3>{}), left(ct_ord<3>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        // less
        {
            BOOST_HANA_CONSTANT_CHECK(less(
                left(ct_ord<0>{}),
                left(ct_ord<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                left(ct_ord<0>{}),
                left(ct_ord<0>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                left(ct_ord<1>{}),
                left(ct_ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(less(
                right(ct_ord<0>{}),
                right(ct_ord<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                right(ct_ord<0>{}),
                right(ct_ord<0>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                right(ct_ord<1>{}),
                right(ct_ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(less(
                left(ct_ord<0>{}),
                right(ct_ord<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(less(
                left(ct_ord<0>{}),
                right(ct_ord<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(less(
                left(ct_ord<1>{}),
                right(ct_ord<0>{})
            ));
        }

        // laws
        test::TestOrderable<Either>{ords};
    }
}
