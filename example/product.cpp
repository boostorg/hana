/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/pair.hpp>

#include <string>
using namespace boost::hana;


int main() {
    //! [comparable]
    BOOST_HANA_RUNTIME_ASSERT(pair(1, std::string{"234"}) == pair(1, "234"));
    BOOST_HANA_CONSTEXPR_ASSERT(pair('x', 2) != pair('y', 2));
    //! [comparable]

    //! [first]
    BOOST_HANA_CONSTEXPR_ASSERT(first(pair(1, 'x')) == 1);
    //! [first]

    //! [second]
    BOOST_HANA_CONSTEXPR_ASSERT(second(pair(1, 'x')) == 'x');
    //! [second]

    //! [make]
    BOOST_HANA_CONSTEXPR_ASSERT(make<Pair>(1, 'x') == pair(1, 'x'));
    //! [make]
}
