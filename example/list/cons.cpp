/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_ASSERT(cons(1, list()) == list(1));
    BOOST_HANA_CONSTEXPR_ASSERT(cons(1, list('2', 3.3)) == list(1, '2', 3.3));
    BOOST_HANA_CONSTEXPR_ASSERT(cons(1, cons('2', cons(3.3, list()))) == list(1, '2', 3.3));
    //! [main]
}
