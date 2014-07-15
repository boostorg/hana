/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical/mcd.hpp>

#include <boost/hana/detail/minimal/logical.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


constexpr struct { } invalid{};

template <typename mcd>
void test() {
    constexpr auto logical = detail::minimal::logical<mcd>;

    BOOST_HANA_STATIC_ASSERT(and_(logical(true)) == logical(true));
    BOOST_HANA_STATIC_ASSERT(and_(logical(false)) == logical(false));

    BOOST_HANA_STATIC_ASSERT(and_(logical(true), logical(true)) == logical(true));
    BOOST_HANA_STATIC_ASSERT(and_(logical(true), logical(false)) == logical(false));
    // BOOST_HANA_STATIC_ASSERT(!and_(logical(false), invalid));

    BOOST_HANA_STATIC_ASSERT(and_(logical(true), logical(true), logical(true)) == logical(true));
    BOOST_HANA_STATIC_ASSERT(and_(logical(true), logical(true), logical(false)) == logical(false));
    // BOOST_HANA_STATIC_ASSERT(and_(logical(true), logical(false), invalid) == logical(false));
    // BOOST_HANA_STATIC_ASSERT(!and_(logical(false), invalid, invalid));

    // operators
    BOOST_HANA_STATIC_ASSERT((logical(true) && logical(true)) == and_(logical(true), logical(true)));
    BOOST_HANA_STATIC_ASSERT((logical(true) && logical(false)) == and_(logical(true), logical(false)));
    BOOST_HANA_STATIC_ASSERT((logical(false) && logical(true)) == and_(logical(false), logical(true)));
    BOOST_HANA_STATIC_ASSERT((logical(false) && logical(false)) == and_(logical(false), logical(false)));
}

int main() {
    test<Logical::mcd>();
}
