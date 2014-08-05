/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/logical.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto logical = detail::minimal::logical<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(not_(logical(true)) == logical(false));
    BOOST_HANA_CONSTEXPR_ASSERT(not_(logical(false)) == logical(true));

    BOOST_HANA_CONSTEXPR_ASSERT(!logical(true) == not_(logical(true)));
    BOOST_HANA_CONSTEXPR_ASSERT(!logical(false) == not_(logical(false)));
}

int main() {
    test<Logical::mcd>();
}
