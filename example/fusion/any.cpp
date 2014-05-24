/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>

#include <cassert>
using namespace boost::hana;


struct odd {
    template <typename T>
    constexpr bool operator()(T t) const { return t % 2; }
};

int main() {
    // constexpr
    BOOST_HANA_STATIC_ASSERT(any(odd{}, list(1, 2)));
    BOOST_HANA_STATIC_ASSERT(!any(odd{}, list(2, 4)));

    // runtime
    int _1 = 1, _2 = 2, _4 = 4;
    assert(any(odd{}, list(_1, _2)));
    assert(!any(odd{}, list(_2, _4)));
}
