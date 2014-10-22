/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/pair.hpp>

#include <boost/hana/detail/assert.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


int main() {
    using test::x;

    // first
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(first(pair(x<1>, x<2>)), x<1>));
    }

    // second
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(second(pair(x<1>, x<2>)), x<2>));
    }

    // make
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<Pair>(x<1>, x<2>),
            pair(x<1>, x<2>)
        ));
    }
}
