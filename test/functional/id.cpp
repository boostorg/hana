/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/assert.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(id(0) == 0);
    BOOST_HANA_CONSTEXPR_ASSERT(id('1') == '1');
    BOOST_HANA_CONSTEXPR_ASSERT(id(2.2) == 2.2);
    BOOST_HANA_CONSTEXPR_ASSERT(id(3.3f) == 3.3f);
}
