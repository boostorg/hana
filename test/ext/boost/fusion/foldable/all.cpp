/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include "../helper.hpp"
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto is_even = [](auto x) {
    return x % 2 == 0;
};

int main() {
    with_nonassociative_forward_sequences([=](auto container) {
        BOOST_HANA_CONSTANT_ASSERT(all(container(), is_even));
        BOOST_HANA_RUNTIME_ASSERT(!all(container(1), is_even));
        BOOST_HANA_RUNTIME_ASSERT( all(container(2), is_even));
        BOOST_HANA_RUNTIME_ASSERT( all(container(2, 4), is_even));
        BOOST_HANA_RUNTIME_ASSERT(!all(container(1, 2), is_even));
        BOOST_HANA_RUNTIME_ASSERT(!all(container(1, 3), is_even));
        BOOST_HANA_RUNTIME_ASSERT(!all(container(1, 3, 4), is_even));
    });
}
