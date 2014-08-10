/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/pair.hpp>

#include <boost/hana/detail/assert.hpp>

#include <utility>
using namespace boost::hana;


int main() {
    // first
    {
        BOOST_HANA_CONSTEXPR_ASSERT(first(std::make_pair(1, '2')) == 1);
    }

    // second
    {
        BOOST_HANA_CONSTEXPR_ASSERT(second(std::make_pair(1, '2')) == '2');
    }

    // make_product
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            std::make_pair(1, '2'),
            make_product<StdPair>(1, '2')
        ));
    }
}
