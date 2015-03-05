/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <laws/iterable.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


struct t1; struct t2; struct t3; struct t4;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto vectors = make<Tuple>(
          mpl::vector<>{}
        , mpl::vector<t1>{}
        , mpl::vector<t1, t2>{}
        , mpl::vector<t1, t2, t3>{}
        , mpl::vector<t1, t2, t3, t4>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(mpl::vector<t1>{}),
                type<t1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(mpl::vector<t1, t2>{}),
                type<t1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(mpl::vector<t1, t2, t3>{}),
                type<t1>
            ));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(mpl::vector<t1>{}),
                mpl::vector<>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(mpl::vector<t1, t2>{}),
                mpl::vector<t2>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(mpl::vector<t1, t2, t3>{}),
                mpl::vector<t2, t3>{}
            ));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(mpl::vector<>{}));
            BOOST_HANA_CONSTANT_CHECK(is_empty(mpl::vector0<>{}));

            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(mpl::vector<t1>{})));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(mpl::vector1<t1>{})));

            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(mpl::vector<t1, t2>{})));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(mpl::vector2<t1, t2>{})));
        }

        // laws
        test::TestIterable<ext::boost::mpl::Vector>{vectors};
    }
}
