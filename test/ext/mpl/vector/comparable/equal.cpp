/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/mpl/vector.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


struct x0; struct x1;

int main() {
    BOOST_HANA_STATIC_ASSERT(equal(mpl::vector<>{}, mpl::vector<>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(mpl::vector<x0>{}, mpl::vector<>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(mpl::vector<>{}, mpl::vector<x0>{}));
    BOOST_HANA_STATIC_ASSERT(equal(mpl::vector<x0>{}, mpl::vector<x0>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(mpl::vector<x0, x1>{}, mpl::vector<x0>{}));
    BOOST_HANA_STATIC_ASSERT(!equal(mpl::vector<x0>{}, mpl::vector<x0, x1>{}));
    BOOST_HANA_STATIC_ASSERT(equal(mpl::vector<x0, x1>{}, mpl::vector<x0, x1>{}));
}
