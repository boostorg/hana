/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % 2;
};

int main() {
    BOOST_HANA_STATIC_ASSERT(all(odd, list(1, 3)));
    BOOST_HANA_STATIC_ASSERT(!all(odd, list(1, 2)));
}
