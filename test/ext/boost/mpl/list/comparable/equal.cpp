/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/list.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <boost/mpl/list.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


struct x0; struct x1;

int main() {
    BOOST_HANA_STATIC_ASSERT(equal(mpl::list<>{}, mpl::list<>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(mpl::list<x0>{}, mpl::list<>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(mpl::list<>{}, mpl::list<x0>{}));
    BOOST_HANA_STATIC_ASSERT(equal(mpl::list<x0>{}, mpl::list<x0>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(mpl::list<x0, x1>{}, mpl::list<x0>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(mpl::list<x0>{}, mpl::list<x0, x1>{}));
    BOOST_HANA_STATIC_ASSERT(equal(mpl::list<x0, x1>{}, mpl::list<x0, x1>{}));
}
