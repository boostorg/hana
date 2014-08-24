/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(
        fill('x', tuple(1, '2', 3.3, nullptr)) == tuple('x', 'x', 'x', 'x')
    );

    BOOST_HANA_CONSTANT_ASSERT(fill('x', nothing) == nothing);
    BOOST_HANA_CONSTEXPR_ASSERT(fill('x', just('y')) == just('x'));
    //! [main]
}
