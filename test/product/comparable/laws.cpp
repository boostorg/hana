/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/product/mcd.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/product.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    constexpr auto prod = detail::minimal::product<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(
        Comparable::laws::check(
            list(
                prod(x<0>, x<1>), prod(x<1>, x<0>),
                prod(x<0>, x<2>), prod(x<1>, x<2>)
            )
        )
    );
}

int main() {
    test<Product::mcd>();
}
