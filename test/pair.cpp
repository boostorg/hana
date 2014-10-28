/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/pair.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/product.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Pair> = tuple(type<Product>);

    template <>
    auto objects<Pair> = tuple(
          pair(x<0>, x<0>)
        , pair(x<0>, x<1>)
        , pair(x<1>, x<0>)
        , pair(x<1>, x<1>)
    );
}}}


int main() {
    test::check_datatype<Pair>();

    // Product
    {
        using test::x;

        // first
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                first(pair(x<1>, x<2>)), x<1>
            ));
        }

        // second
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                second(pair(x<1>, x<2>)), x<2>
            ));
        }

        // make
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                make<Pair>(x<1>, x<2>),
                pair(x<1>, x<2>)
            ));
        }
    }
}
