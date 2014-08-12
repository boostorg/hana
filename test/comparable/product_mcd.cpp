/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/product/mcd.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/minimal_product.hpp>

#include <test/laws/comparable.hpp>
using namespace boost::hana;


template <typename Mcd>
void tests() {
    using test::x;
    auto prod = test::minimal_product<Mcd>;

    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(prod(x<0>, x<1>), prod(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(prod(x<0>, x<1>), prod(x<9>, x<1>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(prod(x<0>, x<1>), prod(x<0>, x<9>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(prod(x<0>, x<1>), prod(x<9>, x<9>))));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            prod(x<0>, x<1>), prod(x<1>, x<0>),
            prod(x<0>, x<2>), prod(x<1>, x<2>)
        ));
    }
}

int main() {
    tests<Product::mcd>();
}
