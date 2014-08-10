/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


struct x1; struct x2; struct x3;

int main() {
    // head
    {
        BOOST_HANA_CONSTANT_ASSERT(head(mpl::vector<x1>{}) == type<x1>);
        BOOST_HANA_CONSTANT_ASSERT(head(mpl::vector<x1, x2>{}) == type<x1>);
        BOOST_HANA_CONSTANT_ASSERT(head(mpl::vector<x1, x2, x3>{}) == type<x1>);
    }

    // tail
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(mpl::vector<x1>{}), mpl::vector<>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(mpl::vector<x1, x2>{}), mpl::vector<x2>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(mpl::vector<x1, x2, x3>{}), mpl::vector<x2, x3>{}));
    }

    // is_empty
    {
        BOOST_HANA_CONSTANT_ASSERT(is_empty(mpl::vector<>{}));
        BOOST_HANA_CONSTANT_ASSERT(is_empty(mpl::vector0<>{}));

        BOOST_HANA_CONSTANT_ASSERT(!is_empty(mpl::vector<x1>{}));
        BOOST_HANA_CONSTANT_ASSERT(!is_empty(mpl::vector1<x1>{}));

        BOOST_HANA_CONSTANT_ASSERT(!is_empty(mpl::vector<x1, x2>{}));
        BOOST_HANA_CONSTANT_ASSERT(!is_empty(mpl::vector2<x1, x2>{}));
    }
}
