/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/list.hpp>

#include <boost/hana/comparable/detail/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/list/instance.hpp>

#include <boost/mpl/list.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


struct x0; struct x1; struct x2;

int main() {
    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(mpl::list<>{}, mpl::list<>{}));
        BOOST_HANA_CONSTANT_ASSERT(!equal(mpl::list<x0>{}, mpl::list<>{}));
        BOOST_HANA_CONSTANT_ASSERT(!equal(mpl::list<>{}, mpl::list<x0>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(mpl::list<x0>{}, mpl::list<x0>{}));
        BOOST_HANA_CONSTANT_ASSERT(!equal(mpl::list<x0, x1>{}, mpl::list<x0>{}));
        BOOST_HANA_CONSTANT_ASSERT(!equal(mpl::list<x0>{}, mpl::list<x0, x1>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(mpl::list<x0, x1>{}, mpl::list<x0, x1>{}));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(
            Comparable::laws::check(
                list(
                    mpl::list<>{}, mpl::list<x0>{}, mpl::list<x0, x1>{},
                    mpl::list<x0, x1, x2>{}, mpl::list<x1, x0, x2>{}
                )
            )
        );
    }
}
