/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;
using namespace literals;


//! [main]
BOOST_HANA_CONSTANT_ASSERT(
    sort(list(1_c, -2_c, 3_c, 0_c)) == list(-2_c, 0_c, 1_c, 3_c)
);
//! [main]

int main() { }
