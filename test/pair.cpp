/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/pair.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/product.hpp>
#include <test/cnumeric.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Pair> = make<Tuple>(type<Product>);

    template <>
    auto objects<Pair> = make<Tuple>(
          pair(cnumeric<int, 0>, cnumeric<int, 0>)
        , pair(cnumeric<int, 0>, cnumeric<int, 1>)
        , pair(cnumeric<int, 1>, cnumeric<int, 0>)
        , pair(cnumeric<int, 1>, cnumeric<int, 1>)
    );
}}}


int main() {
    test::check_datatype<Pair>();

    // Product
    {
        using test::x;

        // first
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                first(pair(x<1>, x<2>)), x<1>
            ));
        }

        // second
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                second(pair(x<1>, x<2>)), x<2>
            ));
        }

        // make
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Pair>(x<1>, x<2>),
                pair(x<1>, x<2>)
            ));
        }
    }
}
