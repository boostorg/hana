/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foreign/comparable.hpp>
#include <boost/hana/foreign/enumerable.hpp>

#include <boost/hana/detail/assert.hpp>

#define BOOST_HANA_TEST_ENUMERABLE
#include <test/builtin_integer.hpp>
using namespace boost::hana;


int main() {
    // succ
    {
        BOOST_HANA_CONSTEXPR_ASSERT(succ(integer{0}) == integer{1});
        BOOST_HANA_CONSTEXPR_ASSERT(succ(integer{1}) == integer{2});
        BOOST_HANA_CONSTEXPR_ASSERT(succ(integer{2}) == integer{3});
    }

    // pred
    {
        BOOST_HANA_CONSTEXPR_ASSERT(pred(integer{3}) == integer{2});
        BOOST_HANA_CONSTEXPR_ASSERT(pred(integer{2}) == integer{1});
        BOOST_HANA_CONSTEXPR_ASSERT(pred(integer{1}) == integer{0});
    }
}
