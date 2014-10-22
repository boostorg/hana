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


struct x1; struct x2; struct x3;

int main() {
    // cons
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(type<x1>, ::boost::mpl::vector<>{}),
            ::boost::mpl::vector<x1>{}
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(type<x1>, ::boost::mpl::vector<x2>{}),
            ::boost::mpl::vector<x1, x2>{}
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(type<x1>, ::boost::mpl::vector<x2, x3>{}),
            ::boost::mpl::vector<x1, x2, x3>{}
        ));
    }

    // nil
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            nil<ext::boost::mpl::Vector>,
            ::boost::mpl::vector<>{}
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            nil<ext::boost::mpl::Vector>,
            ::boost::mpl::vector0<>{}
        ));
    }
}
