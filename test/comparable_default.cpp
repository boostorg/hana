/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/is_a.hpp>
using namespace boost::hana;


int main() {
    struct X { struct hana { using datatype = X; }; };
    struct Y { struct hana { using datatype = Y; }; };
    constexpr X x{};
    constexpr Y y{};

    // Two objects of different data types are unequal by default.
    {
        BOOST_HANA_CONSTANT_ASSERT(are<Comparable, X, Y>);
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(x, y)));
    }

    // No instance is provided when the two objects are of the same data type.
    {
        BOOST_HANA_CONSTANT_ASSERT(not_(are<Comparable, X, X>));
        BOOST_HANA_CONSTANT_ASSERT(not_(are<Comparable, Y, Y>));
    }

    // laws
    {
        // we can't check the laws because `X` and `X` is not comparable
    }
}
