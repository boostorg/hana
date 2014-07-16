/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/pair/instance.hpp>

#include <string>
#include <cassert>
using namespace boost::hana;


int main() {
    //! [main]
    assert(pair(1, std::string{"234"}) == pair(1, "234"));
    BOOST_HANA_STATIC_ASSERT(pair('x', 2) != pair('y', 2));
    //! [main]
}
