/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/pair.hpp>
using namespace boost::hana;


int main() {
    //! [pair]
    BOOST_HANA_CONSTEXPR_ASSERT(first(pair(1, 'x')) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(second(pair(1, 'x')) == 'x');
    //! [pair]

    //! [product]
    BOOST_HANA_CONSTEXPR_ASSERT(make<Pair>(1, 'x') == pair(1, 'x'));
    BOOST_HANA_CONSTEXPR_ASSERT(first(pair(1, 'x')) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(second(pair(1, 'x')) == 'x');
    //! [product]
}
