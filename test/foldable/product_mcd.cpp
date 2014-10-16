/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable/product_mcd.hpp>
#include <boost/hana/product/mcd.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/laws/foldable.hpp>
#include <test/minimal_product.hpp>
using namespace boost::hana;


template <typename Mcd>
void tests() {
    using test::x;
    auto product = test::minimal_product<Mcd>;
    auto f = test::injection([]{});

    // unpack
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(product(x<0>, x<1>), f),
            f(x<0>, x<1>)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Foldable_laws(
            product(x<0>, x<1>),
            product(x<1>, x<0>),
            product(x<2>, x<3>)
        ));
    }
}

int main() {
    tests<Product::mcd>();
}
