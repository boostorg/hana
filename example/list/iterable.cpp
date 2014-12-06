/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_CHECK(head(tuple(1, '2', 3.3)) == 1);
    BOOST_HANA_CONSTEXPR_CHECK(tail(tuple(1, '2', 3.3)) == tuple('2', 3.3));
    BOOST_HANA_CONSTANT_CHECK(!is_empty(tuple(1, '2', 3.3)));
    BOOST_HANA_CONSTANT_CHECK(is_empty(tuple()));
    //! [main]
}
