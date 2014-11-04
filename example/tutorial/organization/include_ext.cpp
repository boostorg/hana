/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>

//! [main]
#include <boost/hana/ext/std/tuple.hpp>
#include <tuple> // still required to create a tuple
using namespace boost::hana;


int main() {
    constexpr std::tuple<int, char, float> xs{1, '2', 3.0f};
    BOOST_HANA_CONSTEXPR_ASSERT(head(xs) == 1);
}
//! [main]
