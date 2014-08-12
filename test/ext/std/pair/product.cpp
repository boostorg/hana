/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/pair.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <utility>
using namespace boost::hana;


int main() {
    using test::x;

    // first
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            first(std::make_pair(x<0>, x<1>)),
            x<0>
        ));
    }

    // second
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            second(std::make_pair(x<0>, x<1>)),
            x<1>
        ));
    }

    // make_product
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            std::make_pair(x<0>, x<1>),
            make_product<StdPair>(x<0>, x<1>)
        ));
    }
}
