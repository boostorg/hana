/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/detail/assert.hpp>
using namespace boost::hana;


int main() {
    auto s1 = BOOST_HANA_STRING("abcd");
    auto s2 = string<'a', 'b', 'c', 'd'>;

    BOOST_HANA_CONSTEXPR_ASSERT(s1.get()[0] == s2.get()[0]);
    BOOST_HANA_CONSTEXPR_ASSERT(s1.get()[1] == s2.get()[1]);
    BOOST_HANA_CONSTEXPR_ASSERT(s1.get()[2] == s2.get()[2]);
    BOOST_HANA_CONSTEXPR_ASSERT(s1.get()[3] == s2.get()[3]);
}
