/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/minimal_product.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>

// instances
#include <test/auto/product.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<MinimalProduct> = tuple(
        type<Product>
    );

    template <>
    auto objects<MinimalProduct> = tuple(
        minimal_product(x<0>, x<0>),
        minimal_product(x<0>, x<1>),
        minimal_product(x<1>, x<0>),
        minimal_product(x<1>, x<1>)
    );
}}}


int main() {
    test::check_datatype<test::MinimalProduct>();
}
