/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>

#include "udt.hpp"
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

int main() {
    constexpr auto s = 3.3;
    // foldl
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldl(udt{0, '0'}, s, f), f(f(s, 0), '0')));
    }

    // foldr
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(foldr(udt{0, '0'}, s, f), f(0, f('0', s))));
    }
}
