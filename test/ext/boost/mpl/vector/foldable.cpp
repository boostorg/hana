/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <laws/base.hpp>
#include <laws/foldable.hpp>

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
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        // unpack
        {
            test::_injection<0> f{};
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(mpl::vector<>{}, f),
                f()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(mpl::vector<t1>{}, f),
                f(type<t1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(mpl::vector<t1, t2>{}, f),
                f(type<t1>, type<t2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(mpl::vector<t1, t2, t3>{}, f),
                f(type<t1>, type<t2>, type<t3>)
            ));
        }

        // laws
        test::TestFoldable<ext::boost::mpl::Vector>{vectors};
    }
}
