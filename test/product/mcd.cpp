/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/product.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <test/minimal_product.hpp>
using namespace boost::hana;


template <typename Mcd>
void tests() {
    constexpr auto prod = test::minimal_product<Mcd>;
    using Prod = Mcd;
    using test::x;

    // first
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(first(prod(x<0>, x<1>)), x<0>));
    }

    // second
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(second(prod(x<0>, x<1>)), x<1>));
    }

    // make
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<Prod>(x<0>, x<1>),
            prod(x<0>, x<1>)
        ));
    }
}

int main() {
    tests<Product::mcd>();
}
