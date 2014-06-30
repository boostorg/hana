/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_array.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <array>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(equal(nil<StdArray>, std::array<char, 0>{}));
    BOOST_HANA_STATIC_ASSERT(equal(nil<StdArray>, std::array<int, 0>{}));
    BOOST_HANA_STATIC_ASSERT(equal(nil<StdArray>, std::array<long, 0>{}));
}
