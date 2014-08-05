/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/logical.hpp>
using namespace boost::hana;


constexpr struct { } invalid{};

template <typename mcd>
void test() {
    constexpr auto logical = detail::minimal::logical<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(true)) == logical(true));
    BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(false)) == logical(false));

    BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(false), logical(false)) == logical(false));
    BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(false), logical(true)) == logical(true));
    // BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(true), invalid));

    BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(false), logical(false), logical(false)) == logical(false));
    BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(false), logical(false), logical(true)) == logical(true));
    // BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(false), logical(true), invalid));
    // BOOST_HANA_CONSTEXPR_ASSERT(or_(logical(true), invalid, invalid));

    // operators
    BOOST_HANA_CONSTEXPR_ASSERT((logical(true) || logical(true)) == or_(logical(true), logical(true)));
    BOOST_HANA_CONSTEXPR_ASSERT((logical(true) || logical(false)) == or_(logical(true), logical(false)));
    BOOST_HANA_CONSTEXPR_ASSERT((logical(false) || logical(true)) == or_(logical(false), logical(true)));
    BOOST_HANA_CONSTEXPR_ASSERT((logical(false) || logical(false)) == or_(logical(false), logical(false)));
}

int main() {
    test<Logical::mcd>();
}
